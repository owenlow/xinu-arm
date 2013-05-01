/**
 * File: tempfs.c
 * Description: Ram-based filesystem for Xinu
 * Author: Owen Royall-Kahin
 */

#include <tempfs.h>
#include <memory.h>
#include <string.h>

/***** Private Definitions *****/



// Cluster size in bytes (should be multiple of 1024)
#define CL_SIZE 8192
#define CL_COUNT 6000

// FS size in bytes
#define FS_SIZE ( CL_SIZE * CL_COUNT )

// DT entry size is 128 bytes
typedef struct dt_entry {
    char name[112];
    uint32_t index; // index of the first cluster for the file
    uint32_t size; // size of the file
    uint32_t type; // 0x0000 = file, 0xFFFF = directory
    uint32_t time; // creation date (unix epoch format)
} dt_entry_t;

/***** Globals *****/
static int tempfs_initialized;
uint32_t fs_index;
uint32_t *boot_record;
dt_entry_t *dir_table;
uint32_t *fat;
uint8_t *fs;

/****** Navigating the Filesystem *****/
int read_data( int, char**, size_t );
void write_data( int, char *, size_t );
int tempread( void*, size_t, size_t );
int tempwrite( const void*, size_t, size_t );
void tempseek( uint32_t );
int get_dt_index( char * );
int get_free_fat();
int get_free_dt();
void clear_fat_chain( int );

/***** Begin Init *****/

int _fs_init() {
    int i;
    fs_index = 0;
    
    if (!fs) {
        // data is the 'disk'
        fs = memget( FS_SIZE );
        
        boot_record = memget(4*sizeof(uint32_t));
        // configure boot record
        // [0] = cluster size
        // [1] = fs size
        // [2] = dt index
        // [3] = fat index
        boot_record[0] = CL_SIZE;
        boot_record[1] = FS_SIZE;
        boot_record[2] = 1;
        boot_record[3] = 2;
        
        printf("fs initialized\nsize: %d\ncluster size:%d\ncluster count:%d\n",
               FS_SIZE, CL_SIZE, CL_COUNT);
        
        int dt_size = CL_SIZE/128;
        dir_table = memget(sizeof(dt_entry_t)*dt_size);
        for (i = 0; i < dt_size; ++i) {
            dir_table[i].name[0] = 0;
        }
        
        fat = memget(CL_COUNT);
        // Clear FAT
        memset(fat, 0x0000, CL_COUNT*sizeof(uint32_t));
        
        // First 3 clusters occupied (BR, DT, FAT)
        memset(fat, 0xFFFF, 3*sizeof(uint32_t));
        
        
        // Write BR
        tempseek(0);
        tempwrite( fat, CL_SIZE, 1 );
        
        // Write DT
        tempseek( boot_record[2]*CL_SIZE );
        tempwrite( dir_table, CL_SIZE, 1 );
        
        // Write FAT
        tempseek( boot_record[3]*CL_SIZE );
        tempwrite( fat, CL_SIZE, 1 );
        
        printFAT();
        
    }
    return 0;
}



int tempread( void* buffer, size_t size, size_t n ) {
    int i;
    
    for (i = 0; i < (n*size); ++i) {
        ((uint8_t*)buffer)[i] = fs[fs_index+i];
    }
    
    fs_index += i;
    return 0;
}

int tempwrite( const void* buffer, size_t size, size_t n ) {
    int i;
    
    for (i = 0; i < (n*size); ++i) {
        fs[fs_index+i] = ((uint8_t*)buffer)[i];
    }
    
    fs_index += i;
    return 0;
}

void tempseek( uint32_t offset ) {
    fs_index = offset;
}

int get_dt_index( char * filename ) {
    int i;
    
    for (i = 0; i != CL_SIZE/sizeof(dt_entry_t); ++i) {
        if (strncmp(dir_table[i].name, filename, 112) == 0) {
            return i;
        }
    }
    
    return -1;
}

int get_free_fat() {
    int i;
    
    for (i = 0; i < CL_COUNT; ++i) {
        if (fat[i] == 0) {
            return i;
        }
    }
    
    return -1;
}

int get_free_dt() {
    int i;
    
    for (i = 0; i < CL_SIZE/sizeof(dt_entry_t); ++i) {
        if (dir_table[i].name[0] == 0 || dir_table[i].name[0] == 0xFF) {
            return i;
        }
    }
    
    return -1;
}

void clear_fat_chain( int start_index ) {
    int current_cluster = start_index;
    
    int next_cluster;
    while (fat[current_cluster] != 0xFFFF) {
        next_cluster = fat[current_cluster];
        fat[current_cluster] = 0x0000;
        current_cluster = next_cluster;
    }
    fat[current_cluster] = 0x0000;
}

int read_data( int cluster_index, char** buffer, size_t size ) {
    int dsize = CL_SIZE;
    uint8_t *data = memget(dsize);
    uint8_t *currentPos = data;
    int i = 0;
    
    uint32_t data_left = size;
    
    while (cluster_index != 0xFFFF) {
        tempseek( cluster_index*CL_SIZE );
        if ( i > 0 ) {
            int newsize = CL_SIZE*(i+1);
            uint8_t temp = memget( newsize );
            memcpy( temp, data, dsize );
            
            memfree( data, dsize );
            dsize = newsize;
            data = temp;
        }
        ++i;
        
        tempread( currentPos, CL_SIZE, 1 );
        cluster_index = fat[cluster_index];
    }
    
    *buffer = data;
    return CL_SIZE*i;
}

void write_data( int first_cluster, char *data, size_t size ) {
    uint8_t *currentPos = data;
    size_t writeSize = size;
    size_t leftToWrite = size;
    
    if (CL_SIZE < size) {
        writeSize = CL_SIZE;
    }
    
    tempseek( first_cluster*CL_SIZE );
    tempwrite( currentPos, 1, writeSize );
    
    leftToWrite -= writeSize;
    
    int thisCluster = first_cluster;
    int nextCluster = 0;
    
    while ( leftToWrite > 0 ) {
        currentPos += writeSize;
        
        if (writeSize > leftToWrite) {
            writeSize = leftToWrite;
        }
        
        nextCluster = get_free_fat();
        fat[thisCluster] = nextCluster;
        
        tempseek( nextCluster*CL_SIZE );
        tempwrite( currentPos, 1, writeSize );
        
        thisCluster = nextCluster;
        leftToWrite -= writeSize;
        
    }
    
    fat[thisCluster] = 0xFFFF;
    
}




fs_file_t openFile( char * filename ) {
    fs_file_t result;
    
    dt_entry_t dt = dir_table[ get_dt_index( filename ) ];
    result.size = dt.size;
    result.data = memget(result.size);
    read_data( dt.index, &result.data, result.size );
    
    return result;
}

void closeFile( fs_file_t file ) {
    memfree( file.data, file.size );
}

void touch( char* filename ) {
    if (get_dt_index(filename) == -1) {
        int dti = get_free_dt();
        int fi = get_free_fat();
        
        int len = strnlen(filename, 50);
        strncpy(dir_table[dti].name, filename, len);
        dir_table[dti].name[len] = 0;
        dir_table[dti].index = fi;
        dir_table[dti].time = 0;
        dir_table[dti].size = 0;
        dir_table[dti].type = 0;
        
        fat[fi] = 0xFFFF;
    }
}

void rm( char* filename ) {
    int dti = get_dt_index(filename);
    
    if (dti != -1) {
        dir_table[dti].name[0] = 0xFF;
        clear_fat_chain( dir_table[dti].index );
    }
}

void ls() {
    int i;
    
    for (i = 0; i < CL_SIZE/sizeof(dt_entry_t); ++i) {
        if (dir_table[i].name[0] != 0 && dir_table[i].name[0] != 0xFF) {
            printf("%s\n", dir_table[i].name);
        }
    }
}

void cat( char* filename ) {
    int dti = get_dt_index( filename );
    
    if (dti != -1) {
        int current_cluster = dir_table[dti].index;
        int size = dir_table[dti].size;
        printf("size of file: %d\n", size);
        char *data = memget(size);
        read_data( current_cluster, &data, size );
        //printf("%s\n", data);
        
        memfree( data, size );
    }
}

void fileappend( char* filename, char** content, int nargs ) {
    int i;
    int dti = get_dt_index( filename );
    
    if (dti != -1) {
        
        for (i = 0; i < nargs; ++i) {
            printf("appending: %s\n", content[i]);
            int size = strnlen( content[i], 50 );
            write_data( dir_table[dti].index, content[i], size );
            dir_table[dti].size += size+1;
        }
    }
}

void df() {
    printf("tempfs Statistics\n");
    printf("Cluster size (bytes): %d\n", CL_SIZE);
    printf("Total Clusters: %d\n", CL_COUNT);
    int used_clusters = 0;
    int i;
    for (i = 0; i < CL_COUNT; ++i) {
        if (fat[i] != 0x00) {
            ++used_clusters;
        }
    }
    printf("Used Clusters: %d\n", used_clusters);
    printf("Available Clusters: %d\n", CL_COUNT-used_clusters);
    double percent = (CL_COUNT-used_clusters)*(100/CL_COUNT);
    printf("Percentage Free: %d\n", percent);
    
    //printFAT();
    //printDT();
}

void write_dt() {
    //tempseek( CL_SIZE*boot_record[2] );
}

void printDT() {
    int i;
    
    printf("Directory Table:\n");
    for (i = 0; i < CL_SIZE/128; ++i) {
        if (dir_table[i].name[0] != 0x00 &&
            dir_table[i].name[0] != 0xff) {
            printf("Entry #%d\n", i);
            printf("\tName: %s\n", dir_table[i].name);
            printf("\tIndex: %d\n", dir_table[i].index);
            printf("\tSize: %d\n", dir_table[i].size);
            printf("\tType: %d\n", dir_table[i].type);
            printf("\tTime: %d\n", dir_table[i].time);
        }
        sleep(1);
    }
}

void printFAT() {
    int i;
    printf("File allocation table:\n");
    for (i = 0; i < 10; ++i) {
        printf("fat[%d]=%04x\n", i, fat[i]);
        sleep(1);
    }
}
