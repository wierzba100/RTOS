#include "string.h"
#define NULL '\0' 

/*char acCopySrc[] = "ABC"; 
char acCopyDst[] = "ABCDEFG"; 

char acCompareStr1[] = "12345"; 
char acCompareStr2[] = "123456"; 

char acAppendSrc[254] = "UST"; 
char acAppendDst[254] = "AGH "; 

char cOldChar = 'A'; 
char cNewChar = 'B'; 
char acReplaceCharacterStr[] = "ABCABC"; */



//Funkcja kopiujaca


void CopyString(char pcCopySrc[], char pcCopyDst[]){
	
	unsigned char ucCharCounter;
	
	for(ucCharCounter=0; pcCopySrc[ucCharCounter] != NULL ; ucCharCounter++){
		pcCopyDst[ucCharCounter]=pcCopySrc[ucCharCounter];
	}
	pcCopyDst[ucCharCounter]= NULL;
}

//Funkcja porównujaca


enum CompResult eCompareString(char pcCompareStr1[], char pcCompareStr2[]){
	
	unsigned char ucCharCounter;
	
	for(ucCharCounter=0; pcCompareStr1[ucCharCounter] != NULL || pcCompareStr2[ucCharCounter] != NULL ; ucCharCounter++){
		if(pcCompareStr1[ucCharCounter] != pcCompareStr2[ucCharCounter])
			return DIFFRENT;
	}
	return EQUAL;
}

	

//Funkcja dodajaca


void AppendString(char pcAppendSrc[], char pcAppendDst[]){
	
	unsigned char ucCharCounter;
	
	for(ucCharCounter=0; pcAppendDst[ucCharCounter] != NULL ; ucCharCounter++){}
	CopyString(pcAppendSrc, pcAppendDst+ucCharCounter);

}


//Funkcja dopisujaca


void ReplaceCharacterInString(char pcReplaceCharacterStr[], char cOldChar, char cNewChar){
	
	unsigned char ucCharCounter;
	
	for(ucCharCounter = 0; pcReplaceCharacterStr[ucCharCounter] !=  NULL ; ucCharCounter++){
		if(cOldChar == pcReplaceCharacterStr[ucCharCounter]){
      pcReplaceCharacterStr[ucCharCounter]=cNewChar;
		}	
	}
}

void UIntToHexStr(unsigned int uiValue, char pcStr[]){
  
	unsigned char ucNibbleCounter;  
	unsigned char ucCurrentNibble; 
	pcStr[0] = '0';
	pcStr[1] = 'x';
	pcStr[6] = NULL;
	
  for(ucNibbleCounter = 0; ucNibbleCounter < 4 ; ucNibbleCounter++){
		ucCurrentNibble = ((uiValue >> (ucNibbleCounter*4)) & 0xF);
		if(ucCurrentNibble > 9){
			pcStr[5-ucNibbleCounter] = ucCurrentNibble -10 + 'A';
		}
    else{
			pcStr[5-ucNibbleCounter] = ucCurrentNibble + '0';
		}
	}
}

enum Result eHexStringToUInt(char pcStr[],unsigned int *puiValue){
	
	unsigned char ucCharacterCounter;
	unsigned char ucCurrentCharacter;
	
	*puiValue = 0;
	if((pcStr[0]!='0') || (pcStr[1]!='x') || (pcStr[2]==NULL)){
		return ERROR;
	}
	for(ucCharacterCounter=2; pcStr[ucCharacterCounter]!=NULL; ucCharacterCounter++){
		ucCurrentCharacter = pcStr[ucCharacterCounter];
		if(ucCharacterCounter==6){
			return ERROR;
		}
		if(ucCurrentCharacter <= '9'){
			ucCurrentCharacter = ucCurrentCharacter-'0';
		}
		else{
			ucCurrentCharacter = ucCurrentCharacter-'A'+10;
		}		
		*puiValue = *puiValue << 4;
		*puiValue = *puiValue | ucCurrentCharacter;
	}
	return OK;
}

void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[]){
	
	unsigned char ucCharCounter; 
	
	for(ucCharCounter = 0 ; pcDestinationStr[ucCharCounter] != NULL ; ucCharCounter++){}
	UIntToHexStr(uiValue, &pcDestinationStr[ucCharCounter]);
}	


