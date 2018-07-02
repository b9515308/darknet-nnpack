
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void write_data_ascii(FILE *out_fp, int data_width, void *data)
{
    unsigned char *p = data;

    if(data_width == 1)
    {
        fprintf(out_fp, "%02x", p[data_width-1]);
        return;
    }

    fprintf(out_fp, "%02x", p[data_width-1]);
    write_data_ascii(out_fp, data_width-1, data);
}

void write_line(FILE *out_fp, int data_width, void *data)
{
    write_data_ascii(out_fp, data_width, data);
    fprintf(out_fp, "\n");
}

int write_lines(FILE *out_fp, int size,  int data_width, void *data)
{
    int i;
    int lines;

    lines = size/data_width;

    for(i = 0 ; i < lines ; i++)
        write_line(out_fp, data_width, data + i*data_width);

    size = size % data_width;
    if(size){
        fprintf(out_fp, "00000000000000000000000000000000\n");
        lines++;
    }
    return lines;

}

void write_layer_ASCII(int number,char *layer_name, int w, int h, int c, int dz, char *exten, void* data, char* d)
{
    FILE *out_fp;
    char str[250];
    char buf[250];
    char tmp[20];
    int ret;
    int size;
    void *pad;
//    char *dir = "./fpga_sim_data/";
    char *dir = d;
    strcpy(str, layer_name);
    sprintf(tmp, "%d", number);
    strcat(str,tmp);
    strcat(str,"_");
    sprintf(tmp, "%d", h);
    strcat(str,tmp);
    strcat(str,"x");
    sprintf(tmp, "%d", w);
    strcat(str,tmp);

    strcat(str,"x");
    sprintf(tmp, "%d", c);
    strcat(str,tmp);

    sprintf(tmp, "_%d", dz);
    strcat(str,tmp);
    strcat(str,exten);
    sprintf(buf,"%s%s",dir,str);
    out_fp = fopen(buf, "w");
    ret = write_lines(out_fp, w*h*c*dz,16 ,data);

    printf("Write %d lines to %s\n", ret, buf);
    fclose(out_fp);
}


void write_to_hex_file(char *file_name, char *dir, int num, int w, int h, int c, int dz)
{
    FILE *fp = fopen(file_name, "r");
    long f_size;    
    void *data;
    size_t ret;
    char name[256];
    char *p;

    strcpy(name, file_name);
    p = strchr(name, '_');
    p = strchr(p+1, '_');
    name[p - name-1] = 0;

    fseek (fp, 0, SEEK_END);   // non-portable
    f_size=ftell(fp);
    rewind(fp);

    data = malloc(f_size);
    ret = fread(data, 1, f_size, fp);
    fclose(fp);
    printf("read %ld Bytes from %s\n", ret, file_name);
    write_layer_ASCII(num, name, w, h, c, dz, ".txt", data, dir);
}

void main(){

    FILE *fp = fopen("quan_fpga_weight0_3x144x1_2.weight", "r");
    long f_size;    
    void *data;
    size_t ret;

    fseek (fp, 0, SEEK_END);   // non-portable
    f_size=ftell(fp);
    rewind(fp);

    data = malloc(f_size);
    ret = fread(data, 1, f_size, fp);
    fclose(fp);
    printf("read %ld Bytes\n", ret);

    write_to_hex_file("conv_qinput0_448x448x3_2.input", "./", 0, 448, 448, 3, 2);   
    write_to_hex_file("quan_fpga_weight0_3x144x1_2.weight", "./", 0, 144, 3, 1, 2);
    write_to_hex_file("conv_qout0_16x200704x1_2.out", "./", 0, 200704, 16, 1, 2);
}
