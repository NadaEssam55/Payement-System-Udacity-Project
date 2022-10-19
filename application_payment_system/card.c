#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#include "card.h"
#include <stdio.h>
#include <string.h>
#define MIN_NAME 20
#define MAX_NAME 24
#define MIN_PAN  16
#define MAX_PAN  19

EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {
    EN_cardError_t NameCheck;
    printf("Enter the card holder name :");
    gets(cardData->cardHolderName);
    size_t length = strlen(cardData->cardHolderName);
    if (length == NULL || length<MIN_NAME || length>MAX_NAME) {
        printf("WRONG NAME FORMAT!");
        NameCheck = WRONG_NAME;
    }
    else {
        printf("WELLCOME %s ! \n", cardData->cardHolderName);
        NameCheck = OK_CARD;
    }
    return NameCheck;
}
//////////////////////////////////////////////////////////
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {
    EN_cardError_t Datecheck;
    printf("Enter the card expiry date in the format MM/YY: \n");
    gets(cardData->cardExpirationDate);
    size_t length = strlen(cardData->cardExpirationDate);
    if (length == NULL) {
        printf("WRONG EXPIRY DATE FORMATE !");
        Datecheck = WRONG_EXP_DATE;
    }
    else if (!(isdigit(cardData->cardExpirationDate[0])) || !(isdigit(cardData->cardExpirationDate[1]))) {
        printf("WRONG EXPIRY DATE FORMATE !");
        Datecheck = WRONG_EXP_DATE;
    }
    else if (cardData->cardExpirationDate[2] != '/') {
        printf("WRONG EXPIRY DATE FORMATE !");
        Datecheck = WRONG_EXP_DATE;
    }
    else if (!(isdigit(cardData->cardExpirationDate[3])) || !(isdigit(cardData->cardExpirationDate[4])))
    {
        printf("WRONG EXPIRY DATE FORMATE !");
        Datecheck = WRONG_EXP_DATE;
    }

    else {
        Datecheck = OK_CARD;
    }
    return Datecheck;
}
//////////////////////////////////////////////////////////
EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
    EN_cardError_t PANCheck;
    printf("Enter the PAN:");
    gets(cardData->primaryAccountNumber);
    size_t length = strlen(cardData->primaryAccountNumber);
    if (length == NULL || length<MIN_PAN || length>MAX_PAN) {
        printf("WRONG PAN FORMATE !");
        PANCheck = WRONG_PAN;
    }
    else {
        PANCheck = OK_CARD;
    }
    return PANCheck;
}