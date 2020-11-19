#include <stdio.h>
#include <string.h>

// Define Mode:
// READ_ONCE - Read the Postal Code file just once and save it in memory. 
// READ_MULTIPLE_TIMES - Open and read the Postal Code file for each person
//#define READ_MULTIPLE_TIMES
#define READ_ONCE

#define NR_POSTALCODES 325413

#if defined (READ_ONCE) && !defined(READ_MULTIPLE_TIMES)
char postal_codes_mem[NR_POSTALCODES][9];
char local_mem [NR_POSTALCODES][40];
#endif

int main(void) {
	    FILE *fp_postal_codes;
	    FILE *fp_people=fopen("random_people_postal_code.txt", "r"); 
	    FILE *fp_output_final=fopen("output_2.txt", "w"); 
	    char *field, *field_line;
	    char buf[128], line[1024], name_postalcode[128];
	    char postal_code_line[16];
	    char postal_code_person[16];
	    char localidade[40];


	    int postalcode_found=0, count_postal_codes, count_people=0;

	    //There are 325413 postal codes and the number of postal codes are in the worst case XXXX-XXX 8 chars
	    int i=0;
	    
	    if (!fp_people) {
		printf("Can't open People file\n");
		return 1;
	    }
	    
	    if (!fp_output_final) {
		printf("Can't open Output file\n");
		return 1;
	    }

	    while (fgets(buf, 128, fp_people)) {	   
	    	strcpy (name_postalcode, buf);	    
		name_postalcode[strlen(name_postalcode)-2]='\0'; 
		field = strtok(buf,"|");
		field = strtok(NULL, "|");
		
		//printf ("FIELD: %s", field);
		strcpy (postal_code_person, field);
		//Replace \n with \0
		postal_code_person[strlen(postal_code_person)-2]='\0'; // Aqui também deu porcaria por causa dos \r\n em Windows VS \n em Unix
			
		//printf ("Postal Code to find: %s\n", postal_code_person);
	
		postalcode_found=0;
		count_people++;
	                
		//Find the Postal Code
#if defined (READ_MULTIPLE_TIMES) && !defined (READ_ONCE)		
		fp_postal_codes = fopen("postal_codes.csv", "r");
		count_postal_codes=0;

		if (!fp_postal_codes) {
			printf("Can't open file\n");
			return 1;
	    	}
		while (postalcode_found==0 && fgets(line, 1024, fp_postal_codes)) {
			if (count_postal_codes != 0) {
				//The village name is in 4th field
				field_line = strtok(line,",");
				for (i=0; i<3; i++) {
					field_line = strtok(NULL, ",");
	        	        }
				strcpy (localidade, field_line);
				
				//The Postal Code are the 14 and 15 fields
				for (i=0; i<11; i++) {
					field_line = strtok(NULL, ",");
	        	        }
				strcpy (postal_code_line, field_line);
	        	        field_line = strtok(NULL, ",");
	        	        strcat(postal_code_line,"-");
				//printf ("%s%s\n", postal_code_line, field_line);

				strcat(postal_code_line,field_line); //O problema estava aqui por causa da 1ª linha que tem mais que 16 chars e corrompia a string seguinte.
			        strcat(postal_code_line,"\0");
							
				//Postal Code found
				if (strcmp(postal_code_line, postal_code_person)==0){
					//printf("FOUND %s!\n", postal_code_person);
					postalcode_found = 1;	
					fprintf(fp_output_final, "%s|%s\n",name_postalcode, localidade);
				}
			}
			count_postal_codes++;
		}

		if (postalcode_found==0){
			printf("PostalCode: %s NOT FOUND, person %d \n", postal_code_person, count_people);
		}
		fclose(fp_postal_codes);
#elif defined (READ_ONCE) && !defined(READ_MULTIPLE_TIMES)
			
		//read the postal codes file just once in the first time the postal code is needed
		if (count_people == 1) {
			fp_postal_codes = fopen("postal_codes.csv", "r"); 		
			count_postal_codes=0;
			
			if (!fp_postal_codes) {
				printf("Can't open file\n");
				return 1;
	    		}
			
			while (fgets(line, 1024, fp_postal_codes)) {
				if (count_postal_codes != 0) {
					//printf ("%s", line);
					//The village name is in 4th field
					field_line = strtok(line,",");
					for (i=0; i<3; i++) {
						field_line = strtok(NULL, ",");
	        		        }
					strcpy (local_mem[count_postal_codes], field_line);
					//printf ("%s\n", field_line);	
					//The Postal Code are the 14 and 15 fields
					for (i=0; i<11; i++) {
						field_line = strtok(NULL, ",");
	        		        }
					strcpy (postal_code_line, field_line);
	        		        field_line = strtok(NULL, ",");
	        		        strcat(postal_code_line,"-");
					
					strcat(postal_code_line,field_line); //O problema estava aqui por causa da 1ª linha que tem mais que 16 chars e corrompia a string seguinte.
				        strcat(postal_code_line,"\0");
					//printf ("Postal Code Line: %s", postal_code_line);
					//printf ("Postal Code Person: %s\n", postal_code_person);
					strcpy (postal_codes_mem[count_postal_codes], postal_code_line);				
				}
				count_postal_codes++;
			}
			fclose(fp_postal_codes);
		}

		for (i = 0; i < NR_POSTALCODES && postalcode_found==0; i++) {
			if (strcmp(postal_codes_mem[i], postal_code_person)==0){
				//printf("FOUND %s!\n", postal_code_person);
				postalcode_found = 1;	
				fprintf(fp_output_final, "%s|%s\n",name_postalcode, local_mem[i]);
				//printf("%s|%s\n",name_postalcode, local_mem[i]);
			}		
		}
		//printf ("%s - %s\n", postal_codes_mem[test],local_mem[test]);

#else
	       printf ("You must define the mode:\n READ_ONCE - Read the Postal Code file just once and save it in memory.\n READ_MULTIPLE_TIMES - Open and read the Postal Code file for each person\n");
	       return 0;
#endif //READ_MULTIPLE_TIMES		
		

	    }
	    printf("END\n");
	    fclose(fp_people);
	    fclose(fp_output_final);
	    
	   return 0;
}
