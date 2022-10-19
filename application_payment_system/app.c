#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include "app.h"
extern ST_accountsDB_t database1[MAX_ACCOUNTS_NUMBER];
extern accnum;
extern sequence_num;
extern ST_transaction database2[MAX_ACCOUNTS_NUMBER];
void appStart(void) {
    ST_transaction serverdata;
    EN_cardError_t cardresult;
    cardresult = getCardHolderName(&serverdata.cardHolderData);
    if (cardresult == WRONG_NAME) {
        exit(0);
    }
    cardresult = getCardExpiryDate(&serverdata.cardHolderData);
    if (cardresult == WRONG_EXP_DATE) {
        exit(0);
    }
    cardresult = getCardPAN(&serverdata.cardHolderData);
    if (cardresult == WRONG_PAN) {
        exit(0);
    }
    ////////////////////////////////////
    EN_terminalError_t terminalresult;
    terminalresult = getTransactionDate(&serverdata.terminalData);
    if (terminalresult == WRONG_DATE) {
        exit(0);
    }
    terminalresult = isCardExpired(&serverdata.cardHolderData, &serverdata.terminalData);
    if (terminalresult == EXPIRED_CARD) {
        exit(0);
    }
    terminalresult = setMaxAmount(&serverdata.terminalData);
    if (terminalresult == INVALID_MAX_AMOUNT) {
        exit(0);
    }
    terminalresult = getTransactionAmount(&serverdata.terminalData);
    if (terminalresult == INVALID_AMOUNT) {
        exit(0);
    }
    terminalresult = isBelowMaxAmount(&serverdata.terminalData);
    if (terminalresult == EXCEED_MAX_AMOUNT) {
        exit(0);
    }
    //////////////////////////////////////////

    EN_transStat_t serverresult;
    if (isValidAccount(&serverdata.cardHolderData) == ACCOUNT_NOT_FOUND) {
        printf("INVALID ACCOUNT \n");
    }
    else {
        printf("ACCOUNT EXISTES IN THE BANK DATABASE \n");
        if (isValidAccount(&serverdata.cardHolderData) == ACCOUNT_BLOCKED) {
            printf("ACCOUNT IS BLOCKED ,STOLEN CARD \n");
        }
        else {
            if (isAmountAvailable(&serverdata.terminalData) == LOW_BALANCE) {
                printf("INSUFICIENT FUND ,the balance is %f \n", database1[accnum].balance);
            }
            else {
                printf("TRANSACTION AMOUNT IS AVAILABLE IN THE BALANCE \n");
            }
        }
    }
    serverresult = recieveTransactionData(&serverdata);
    if (serverresult == DECLINED_STOLEN_CARD) {
        printf("TRANSACTION DECILIND ,STOLEN CARD \n");
    }
    else if (serverresult == ACCOUNT_INVALID) {
        printf("TRANSACTION DECILIND ,INVALID ACCOUNT \n");
    }
    else if (serverresult == DECLINED_INSUFFECIENT_FUND) {
        printf("TRANSACTION DECILIND ,INSUFFECIENT FUND \n");
    }
    else if (serverresult == INTERNAL_SERVER_ERROR) {
        printf("TRANSACTION DECILIND,INTERNAL SERVER ERROR \n");
    }
    else {
        printf("TRANSACTION APPROVED \n");
    }
    saveTransaction(&serverdata);
    getTransaction(sequence_num, &serverdata);
}

