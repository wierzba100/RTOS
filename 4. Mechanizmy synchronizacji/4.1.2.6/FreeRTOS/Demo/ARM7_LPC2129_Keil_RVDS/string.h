
void CopyString(char pcCopySrc[], char pcCopyDst[]);
enum CompResult eCompareString(char pcCompareStr1[], char pcCompareStr2[]);
void AppendString(char pcAppendSrc[], char pcAppendDst[]);
void ReplaceCharacterInString(char pcReplaceCharacterStr[], char cOldChar, char cNewChar);
enum CompResult {DIFFRENT ,EQUAL}; 

void UIntToHexStr(unsigned int uiValue, char pcStr[]);
enum Result eHexStringToUInt(char pcStr[], unsigned int *puiValue);
void AppendUIntToString (unsigned int uiValue, char pcDestinationStr[]);
enum Result {OK, ERROR};

