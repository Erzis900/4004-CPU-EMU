#include <stdio.h>
#include <stdbool.h>

struct {
    unsigned int pc;

    unsigned int s1;
    unsigned int s2;
    unsigned int s3;

    unsigned int acc;
    unsigned int carry;

    unsigned int R0;
    unsigned int R1;
    unsigned int R2;
    unsigned int R3;
    unsigned int R4;
    unsigned int R5;
    unsigned int R6;
    unsigned int R7;
    unsigned int R8;
    unsigned int R9;
    unsigned int RA;
    unsigned int RB;
    unsigned int RC;
    unsigned int RD;
    unsigned int RE;
    unsigned int RF;
} reg;

bool exec_opcode(unsigned char opcode, unsigned char OPR, unsigned char OPA, unsigned int temp_acc, unsigned int temp_carry) {
    switch(OPR) {
        case 0x6: //INC
            OPA = opcode & 0x0F;
            *(&reg.R0 + OPA) += 1;

            return true;
            break;
        case 0xA: //LD
            OPA = opcode & 0x0F;
            reg.acc = *(&reg.R0 + OPA);

            return true;
            break;

        case 0xB: //XCH
            temp_acc = reg.acc;

            OPA = opcode & 0x0F;

            reg.acc = *(&reg.R0 + OPA);
            *(&reg.R0 + OPA) = temp_acc;

            return true;
            break;

        case 0xD: //LDM
            reg.acc = opcode & 0x0F;

            return true;
            break;

        default:
            break;
    }

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
            printf("0x%X opcode not found\n", opcode);
            return false;
    }

    return true;
}

int main() {
    unsigned char ram[640];

    unsigned int temp_acc;
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

    unsigned char OPR;
    unsigned char OPA;

    unsigned int sets = 0;

    for(;;) {
        opcode = ram[reg.pc];
        OPR = opcode >> 4;

        if(cycles == sizeof(ram) * sets) {
            reg.pc = 0;
            sets++;
        }

        if(!exec_opcode(opcode, OPR, OPA, temp_acc, temp_carry)) {
            break;
        }

        reg.pc++;
        cycles++;

        printf("[0x%X] cycles: %d\n", opcode, cycles);
    }
}