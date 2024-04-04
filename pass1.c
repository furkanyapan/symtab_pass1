#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

char opcode[10], operand[10], label[10], t1[20], t2[20], t3[10];
int locctr, start, s = -1, o = -1, i, j = 0, flag, opd;
FILE *inputFile, *optableFile, *symtabFile;

struct SYMTAB
{
    char label[10];
    int addr;
} ST[30];

struct OPTAB
{
    char opcode[10], hexcode[10];
} OT[30];


void read_line();

int main()
{
    inputFile = fopen("input.txt", "r");
    optableFile = fopen("optab.txt", "r");
    symtabFile = fopen("symtab.txt", "w");
    
    // opcode tablosunu oku
    while (1)
    {
        o++;
        fscanf(optableFile, "%s%s", OT[o].opcode, OT[o].hexcode);  
        if (feof(optableFile))
            break;
    }
    
    fscanf(inputFile, "%s%s%x", label, opcode, &opd);  //s string, x hexadecimal
    if (strcmp(opcode, "START") == 0)
    {
        start = opd;
        locctr = start;
        read_line(); // bir sonraki satırı oku
    }
    else
        locctr = 0;

    // opcode'da END görene kadar tekrar et
    while (strcmp(opcode, "END") != 0)
    {
        if (strcmp(label, "") != 0) // Eğer label varsa
        {
            for (i = 0; i <= s; i++)
            {
                if (strcmp(ST[i].label, label) == 0) // Eğer sembol tablosunda varsa error mesaşı ve programdan çık
                {
                    printf("Error");
                    exit(0);
                }
            }
            s++;
            strcpy(ST[s].label, label);   // label'ı sembol tablosunun labelına kopyala
            ST[s].addr = locctr; // locctr'in değerini sembol tablosunun adresşne ata.
        }

        // OPTABLE' da opcode ara
        flag = 0;
        for (i = 0; i < o; i++)
        {
            if (strcmp(opcode, OT[i].opcode) == 0) // Eğer opcode OPTAB'da varsa
            {
                locctr += 0x3;   // location counter'a hex 3 ekle
                flag = 1;
                break;
            }
        }
        if (flag == 0)  // opcode OPTAB'da yoksa 
        {
            if (strcmp(opcode, "WORD") == 0) // Eğer opcode "WORD" ise location counter'a hex 3 ekle
                locctr += 0x3;
            else if (strcmp(opcode, "RESW") == 0) // Eğer opcode "RESW" ise 
                locctr += (0x3 * (atoi(operand))); // location counter'a hex 3 ile hex operandın çarpımını ekle // atoi stringi integer yapar
            else if (strcmp(opcode, "RESB") == 0) // Eğer opcode "RESB" ise
                locctr += atoi(operand); // // location counter'a hex operandı ekle           
            else if (strcmp(opcode, "BYTE") == 0) // Eğer opcode = "BYTE" ise
            {
                int len = strlen(operand);   // operand uzunluğu
                if (operand[0] == 'C')   // karakter  
                    len = len - 3;      // her karakter 1 bayt yer kaplar C,','  3 bayt olduğu için çıkarırız  // C'EOF' -> 3       
                else                    // X // hexadecimal
                    len = (len - 3) / 2;  // bölü 2  X'F1' -> 1
                locctr += len; // location counter'a lenghti ekle
            }
        }

        read_line(); // input.txt'den yeni satırı oku
    }

    // ST'nin içeriğini symtab.txt ye yaz.
    for (i = 0; i <= s; i++)
    {
        fprintf(symtabFile, "%x\t%s\n", ST[i].addr, ST[i].label);
    }

    fclose(inputFile);
    fclose(optableFile);
    fclose(symtabFile);
}

void read_line()
{
    strcpy(t1, ""); // t text
    strcpy(t2, "");
    strcpy(t3, "");

    fscanf(inputFile, "%s", t1);  // ilk kelime 
    if (fgetc(inputFile) != '\n')    
    {
        fscanf(inputFile, "%s", t2); // ikinci kelime 
        if (fgetc(inputFile) != '\n')
        {
            fscanf(inputFile, "%s", t3); // üçüncü kelime 
        }
    }

    if (strcmp(t1, "") != 0 && strcmp(t2, "") != 0 && strcmp(t3, "") != 0) // t1, t2, t3 varsa 
    {
        strcpy(label, t1);      // t1 label'e kopyalanır,
        strcpy(opcode, t2);     // t2 opcode'a kopyalanır,
        strcpy(operand, t3);    // t3 operand'a kopyalanır.
    }

    else if (strcmp(t1, "") != 0 && strcmp(t2, "") != 0 && strcmp(t3, "") == 0)   // t1,t2 varsa ve t3 yoksa
    {
        strcpy(label, "");     
        strcpy(opcode, t1);  // t1 opcode'a kopyalanır,
        strcpy(operand, t2); // // t2 operand'a kopyalanır.
    }

    else if (strcmp(t1, "") != 0 && strcmp(t2, "") == 0 && strcmp(t3, "") == 0) // t1 varsa ve t2,t3 yoksa
    {
        strcpy(label, "");
        strcpy(opcode, t1); // t1 opcode'a kopyalanır.
        strcpy(operand, "");
    }
}