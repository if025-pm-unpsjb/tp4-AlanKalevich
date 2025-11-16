#include "mbed.h"

Serial pc(USBTX, USBRX); // tx, rx

int main() {
    pc.baud(9600);

    while(1){
    	pc.putc(pc.getc());
    }
}
