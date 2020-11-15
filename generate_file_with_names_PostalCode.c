#include <stdio.h>
#include <string.h>

//This needs to be a global variable, otherwise the stack will PUM!
//There are 325413 postal codes and the number of postal codes are in the worst case XXXX-XXX 8 chars, plus \0, it is 9 
char postal_code[325413][9];

int main(void) {
	    FILE *fp = fopen("codigos_postais.csv", "r");
	    char buf[1024];
	    char *field;
	    char names[16][8] = {"Valter", "Mário", "João", "André", "Paulo", "Josefa", "Amandio", "Armando", "Alfredo", "Sebas", "Toino", "Nuria", "Xeila", "Sofia", "Petra", "Maria"};
	    //There are 325413 postal codes and the number of postal codes are in the worst case XXXX-XXX 8 chars
	    int i=0, x=0;

	    if (!fp) {
		printf("Can't open file\n");
		return 1;
	    }

	    while (fgets(buf, 1024, fp)) {
	        //printf("%s\n", buf);
		//return the first field of the csv line
		field = strtok(buf,",");
		
		//We want to get the 14 and 15 fields
		for (i=0; i<14; i++) {		
			field = strtok(NULL, ",");
		}
		strcpy (postal_code[x], field);
		field = strtok(NULL, ",");
		strncat(postal_code[x],"-",1);
		strncat(postal_code[x],field,strlen(field));
	        strncat(postal_code[x],"\0",1);	
		//printf ("%s\n", postal_code[x]);
		x++;
	    }

	    fclose(fp);
	    fp = fopen("random_people_postal_code.txt", "w");

	    if (!fp) {
		    printf("Can't open file\n");
		    return 1;
	    }

	    //Write 500000 random names associated with a Postal Code
	    for (i=0; i<500000; i++) {
	    	fprintf (fp,"%s|%s\n", names[rand()%16], postal_code[rand()%325411]);
	    }
	    
	   return 0;
}
