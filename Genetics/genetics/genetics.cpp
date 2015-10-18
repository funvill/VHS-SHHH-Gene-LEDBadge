// 
// Genetics. This application was created to help me understand how simple 
// Genetics works. I have not gone to school for this and I am new to biology. 
// I suggest that you take what you see here with a grain of salt. 
//                 == I have no idea what I am doing ==
// 
// Last updated: 09 Oct 2015 
// Created by: Steven Smethurst 
//
//
// Based off of: 
// - https://en.wikipedia.org/wiki/Punnett_square
// - https://www.youtube.com/watch?v=swXg3FJhYoY 
// 

#include "stdafx.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */

bool getBit(unsigned char byte, int position) { // position in range 0-7
	return ((byte >> position) & 0x1) > 0 ;
}

unsigned char setBit(unsigned char byte, int position, bool value ) {
	byte ^= (-value ^ byte) & (1 << position);
	return byte; 
}

//
// This class attempts to repercent a simple gene. A gene is a trait in the 
// system. A gene consists of two bits of information also called "allele". 
// A allele can be a dominant (repersented with a Capital letter) or a 
// ressessive (repersented with a lower case letter). 
// If one or more of the allele are dominant, the dominant trait 
// will be present in the system. 
// 
// Below is Punnett square of mating Genes
// https://en.wikipedia.org/wiki/Punnett_square
//
//    A | a    Two Dominant Helerozyous Genotype mate 
//   *--*--*
// A |AA|Aa|   AA = Dominant Homozygous.     25% 
//---*--*--*   Aa = Dominant Helerozyous.    50%
// a |Aa|aa|   aa = Ressessive Homozygous.   25% 
//   *--*--*
//
//    a | a    One Helerozyous Genotype mates with a reccessive Homozygous
//   *--*--*
// A |Aa|Aa|   AA = Dominant Homozygous.      0% 
//---*--*--*   Aa = Dominant Helerozyous.    50%
// a |aa|aa|   aa = Ressessive Homozygous.   50% 
//   *--*--*
//
//    a | a    Two Ressessive Homozygous Genotype mate
//   *--*--*
// a |aa|aa|   AA = Dominant Homozygous.      0% 
//---*--*--*   Aa = Dominant Helerozyous.     0%
// a |aa|aa|   aa = Ressessive Homozygous.  100% 
//   *--*--*
// 
//    A | A    Two Dominant Homozygous Genotype mate
//   *--*--*
// A |AA|AA|   AA = Dominant Homozygous.    100% 
//---*--*--*   Aa = Dominant Helerozyous.     0%
// A |AA|AA|   aa = Ressessive Homozygous.    0% 
//   *--*--*
class Gene
{
	public:
		// This is a global uniquee ID for this gene 
		unsigned int m_id;

		// Genotype 
		// Each gene has two Allele repersented as two bits in this value. 
		unsigned char m_genotype;
		
		Gene() {
			// Every gene will have a global unique ID
			static unsigned int globalID = 0;
			this->m_id = globalID++;
			
			// Randomize the genotype to start. 
			this->m_genotype = 0;
			this->Set(rand() % 4);
		}
		
		// Force the genotype to a perdiculare value 
		void Set(unsigned char genotype) {
			this->m_genotype = genotype;
		}
		unsigned char Get() {
			return this->m_genotype;
		}

		void Mate(Gene &other) {

			printf("Breeding:  ");
			this->Print(); 
			printf(" and ");
			other.Print(); 
			printf(" ");

			unsigned char otherGenotype = other.Get();
			int choice = rand() % 4;

			// Print the Punnett square for debug
			/*
			printf("\n" );
			printf("    %d  | %d  \n", getBit(otherGenotype, 0), getBit(otherGenotype, 1)); 
			printf("  *----*----*\n" );
			printf(" %d| %d%d | %d%d |\n", getBit(this->m_genotype, 0), getBit(this->m_genotype, 0), getBit(otherGenotype, 0), getBit(this->m_genotype, 0), getBit(otherGenotype, 1));
			printf(" %d| %d%d | %d%d |\n", getBit(this->m_genotype, 1), getBit(this->m_genotype, 1), getBit(otherGenotype, 0), getBit(this->m_genotype, 1), getBit(otherGenotype, 1));
			printf("  *----*----*\n\n");
			printf("Choice=%d\n", choice);
			*/
			
			switch (choice) {
			case 0:
				this->m_genotype = setBit(this->m_genotype, 1, getBit(otherGenotype, 0));
				break;
			case 1:
				this->m_genotype = setBit(this->m_genotype, 1, getBit(otherGenotype, 1));
				break;
			case 2:
				this->m_genotype = setBit(this->m_genotype, 0, getBit(m_genotype,    1));
				this->m_genotype = setBit(this->m_genotype, 1, getBit(otherGenotype, 0));
				break;
			case 3:
				this->m_genotype = setBit(this->m_genotype, 0, getBit(m_genotype,    1));
				this->m_genotype = setBit(this->m_genotype, 1, getBit(otherGenotype, 1));
				break;
			}

			printf("Result: ");
			this->Print();
			printf("\n");
		}

		// Phenotype: The trate that is present. Shows the dominant Allele 
		// In this case the Zero is the recessive allele and the 1 is the dominant Allele 
		// 1,1 = (1     ) | Active    | (Homozygous dominant)
		// 1,0 = (1 or 0) | Active    | (Helerozyous)
		// 0,1 = (1 or 0) | Active    | (Helerozyous)
		// 0,0 = (0     ) | NotActive | (Homozygous recessive) 		
		bool IsActive() {
			return (this->m_genotype != 0);
		}


		void Print() {
			printf("G=%d,p=%d%d(%d) ", this->m_id, (this->m_genotype & 1)>0, (this->m_genotype & 2)>0, this->IsActive() );
		}
};

#define DEVICE_MAX			10
#define GENE_ROTATE_RIGHT	0	 
#define GENE_TAIL_LENGTH	1	 
#define GENE_MAX			1 


class Device
{
	public:
		unsigned int m_id;
		Gene m_genes[GENE_MAX];		
		
		unsigned int m_stat_mate_count; 
	
		Device() {
			static unsigned int globalID = 0;
			this->m_id = globalID++;

			m_stat_mate_count = 0; 
		}

		void Print() {
			printf("D=%02d,m=%02d:", this->m_id, m_stat_mate_count);

			for (int geneOffset = 0; geneOffset < GENE_MAX; geneOffset++) {
				// printf("%d,", this->m_genes[geneOffset].IsActive());
				this->m_genes[geneOffset].Print(); 
				printf(" ");
			}

			// printf("%d,", this->m_genes[GENE_ROTATE_RIGHT ].IsActive());
			// printf("%d,", this->m_genes[GENE_TAIL_LENGTH  ].IsActive());
			// printf("\n");
		}

		void Mate(Device &other) {
			m_stat_mate_count++; 
			for (unsigned int offset = 0; offset < GENE_MAX; offset++) {
				this->m_genes[offset].Mate(other.m_genes[offset]); 
			}
		}
};


void test() {
	Gene mother;
	Gene father;

	for (int m = 0; m <= 3; m++) {
		for (int f = 0; f <= 3; f++) {
			mother.Set(m);
			father.Set(f);

			mother.Print(); 
			father.Print(); 

			mother.Mate(father);
			printf("\n");
		}
	}
}

int main()
{
	Device devicePool[DEVICE_MAX];

	for (int j = 0; j < 5; j++) {
		// BREED 
		for (int count = 0; count < 10; count++) {
			int offsetOne = rand() % DEVICE_MAX;
			int offsetTwo = rand() % DEVICE_MAX;
			devicePool[offsetOne].Mate(devicePool[offsetTwo]);
		}

		//// Print info 
		//for (int offset = 0; offset < DEVICE_MAX; offset++) {
		//	devicePool[offset].Print();
		//    printf("\n");
		//}

		printf("\n%d\n", j);
	}

    return 0;
}

