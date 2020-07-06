#include <stdio.h>
#include <stdbool.h>

struct {
        unsigned int pc : 12;

        unsigned int s1 : 12;
        unsigned int s2 : 12;
        unsigned int s3 : 12;

        unsigned int acc : 4;
        unsigned int carry : 1;

        unsigned int R0 : 4;
        unsigned int R1 : 4;
        unsigned int R2 : 4;
        unsigned int R3 : 4;
        unsigned int R4 : 4;
        unsigned int R5 : 4;
        unsigned int R6 : 4;
        unsigned int R7 : 4;
        unsigned int R8 : 4;
        unsigned int R9 : 4;
        unsigned int RA : 4;
        unsigned int RB : 4;
        unsigned int RC : 4;
        unsigned int RD : 4;
        unsigned int RE : 4;
        unsigned int RF : 4;
    } reg;

bool exec_opcode(unsigned char opcode, unsigned int temp_carry) {
    switch(opcode) {
        case 0x00: //NOP
            break;
        case 0xF0: //CLB
            reg.acc = 0;
            reg.carry = 0;
            break;
        case 0xF1: //CLC
            reg.carry = 0;
            break;
        case 0xF2: //IAC
            reg.acc++;
            break;
        case 0xF3: //CMC
            reg.carry = !reg.carry;
            break;
        case 0xF4: //CMA
            reg.acc ^= 0xF;
            break;
        case 0xF5: //RAL
            temp_carry = reg.carry;

            reg.carry = reg.acc >> 3; //set carry to MSB
            reg.acc = reg.acc << 1; //shift accumulator left by 1

            temp_carry == 1 ? (reg.acc |= 1) : (reg.acc &= ~1); //set LSB to old carry
            break;
        case 0xF6: //RAR
            temp_carry = reg.carry;

            reg.carry = reg.acc & 1; //set carry to LSB
            reg.acc = reg.acc >> 1; //shift accumulator right by 1

            temp_carry == 1 ? (reg.acc |= 1 << 3) : (reg.acc &= ~(1 << 3)); //set MSB to old carry
            break;
        case 0xF7: //TCC
            reg.acc = reg.carry;

            //for (int i = 3; i >= 0; i--)  
            //    printf("%d", (reg.acc >> i) & 1);
            
            //printf("\n%d\n", reg.carry);

            reg.carry = 0;
            break;
        case 0xF8: //DAC
            reg.acc--;
            break;
        case 0xFA: //STC
            reg.carry = 1;
            break;
        default:
            printf("0x%x opcode not found\n", opcode);
            return false;
    }

    return true;
}

int main() {
    unsigned char ram[640];

    unsigned int temp_carry;

    for(unsigned int i = 0; i <= 640; i++) {
        ram[i] = 0x00;
    }

    //ram[0] = 0xF3;
    //ram[1] = 0xF6;
    //ram[2] = 0xF4;

    unsigned int cycles = 0;
    reg.pc = 0;

    unsigned char opcode;
    unsigned int sets = 0;

    for(;;) {
        opcode = ram[reg.pc];

        if(cycles == sizeof(ram) * sets) {
            reg.pc = 0;
            sets++;
        }

        if(!exec_opcode(opcode, temp_carry)) {
            break;
        }

        reg.pc++;
        cycles++;

        printf("[0x%x] cycles: %d\n", opcode, cycles);
    }
}