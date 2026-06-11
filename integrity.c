#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define BSIZE 512
#define TEST_BLOCKS 10000 // Bạn có thể tăng lên 10000 để test Double Indirect

void test_integrity() {
    int fd;
    int i;
    char write_buf[BSIZE];
    char read_buf[BSIZE];

    printf("--- Bat dau Test Toan ven du lieu ---\n");

    fd = open("integrity.dat", O_CREATE | O_WRONLY);
    if(fd < 0) {
        printf("Loi: Khong tao duoc file\n");
        exit(1);
    }

    // 1. GHI DU LIEU CO QUY LUAT
    for(i = 0; i < TEST_BLOCKS; i++) {
        // Dien vao moi block mot ky tu khac nhau dua tren chi so i
        memset(write_buf, (i % 26) + 'A', BSIZE); 
        if(write(fd, write_buf, BSIZE) != BSIZE) {
            printf("Loi ghi tai block %d\n", i);
            break;
        }
        if(i % 500 == 0) printf("Dang ghi block %d...\n", i);
    }
    close(fd);

    // 2. DOC VA KIEM TRA
    fd = open("integrity.dat", O_RDONLY);
    printf("\nDang kiem tra lai du lieu...\n");

    for(i = 0; i < TEST_BLOCKS; i++) {
        if(read(fd, read_buf, BSIZE) != BSIZE) {
            printf("Loi doc tai block %d\n", i);
            break;
        }

        // Kiem tra ky tu dau tien cua block xem co dung quy luat (i % 26) + 'A'
        char expected = (i % 26) + 'A';
        if(read_buf[0] != expected) {
            printf("SAI DU LIEU tai block %d: Mong doi '%c', Thuc te '%c'\n", i, expected, read_buf[0]);
            close(fd);
            return;
        }
    }

    printf("\nCHUC MUNG: Toan ven du lieu duoc dam bao tren %d blocks!\n", TEST_BLOCKS);
    close(fd);
}

int main() {
    test_integrity();
    exit(0);
}
