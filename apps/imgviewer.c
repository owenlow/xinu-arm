/**
 * Implementation file for open_image shell command
 */




#define SUCCESS 0
#define NOT_IMAGE 1
#define NOT_FOUND 2

#define BM 0x424d
#define IMGSTART_OFFSET 10

uint16_t *file = {42,4D,76,00,00,00,00,00,00,00,36,00,00,00,28,00,00,00,04,00,00,00,FC,FF,FF,FF,01,00,20,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,FF,00,00,00,FF,00,00,00,FF,00,00,00,FF,00,00,00,FF,00,00,00,FF,00,00,00,FF,00,00,00,FF,00,00,00,FF,00,00,00,FF,00,00,00,FF,00,00,00,FF,00,00,00,FF,00,00,00,FF,00,00,00,FF,00,00,00,FF};

int open_image(char *filename) {
    
    FILE *f = fopen(filename, "r");
    
    uint16_t sigbuffer;
    uint32_t buffer;
    fpos_t byteno = 0;
    
    int img_data_offset;
    
    if (f == NULL) {
        return NOT_FOUND;
    }
    
    if (!read(&sigbuffer, 1, 2, f)) {
        byteno += 2;
        if (sigbuffer == BM) {
            
            while (!read(&buffer, 1, 4, f)) {
                byteno += 4;
                
                switch (byteno) {
                    case IMGSTART_OFFSET:
                        img_data_offset = byteno + buffer;
                        break;
                        
                    default:
                        break;
                }
            }
            
            // EOF Reached
            return SUCCESS;
        }
    }
    
    return NOT_IMAGE;
}