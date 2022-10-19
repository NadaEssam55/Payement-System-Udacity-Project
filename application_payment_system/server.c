#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#include "server.h"
#include "terminal.h"
#include <string.h>
#include<stdio.h>
#include <time.h>
ST_accountsDB_t database1[MAX_ACCOUNTS_NUMBER] = {
{56000,"98561234125563031",RUNNING},
{6200,"78561234785563423",BLOCKED},
{12000,"15691234785699697",RUNNING},
{1720,"86542347856454116",RUNNING},
{5900,"56161056789622131",BLOCKED},
{8000,"89234434569321723",RUNNING},
{57000,"39841123269871145",RUNNING},
{32000,"22145323568632488",RUNNING},
};
ST_transaction database2[MAX_ACCOUNTS_NUMBER] = { 0 };
EN_transStat_t recieveTransCheck;
uint32_t accnum = 0;
uint32_t sequence_num = 0;


EN_serverError_t isValidAccount(ST_cardData_t* cardData) {
    int size = sizeof(database1) / sizeof(database1[0]);
    uint32_t flag = 0;
    EN_serverError_t ValidaccCheck;
    for (accnum = 0; accnum < size; accnum++) {
        if (strcmp(cardData->primaryAccountNumber, database1[accnum].primaryAccountNumber) == 0) {
            flag = 1;
            break;
        }
        else {
            flag = 0;
        }
    }
    if (flag == 1) {
        if (database1[accnum].state == BLOCKED) {
            ValidaccCheck = ACCOUNT_BLOCKED;
        }
        else {
            ValidaccCheck = SERVER_OK;
        }
    }
    else {
        ValidaccCheck = ACCOUNT_NOT_FOUND;
    }
    return  ValidaccCheck;
}
///////////////////////////////////////////////////////////////
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData) {
    EN_serverError_t AmountavailabeCheck;
    if (termData->transAmount > database1[accnum].balance) {

        AmountavailabeCheck = LOW_BALANCE;
    }
    else {
        AmountavailabeCheck = SERVER_OK;
    }
    return AmountavailabeCheck;
}
//////////////////////////////////////////////////////////////////
EN_serverError_t saveTransaction(ST_transaction* transData) {
    EN_serverError_t SavetransCheck;
    transData->transactionSequenceNumber = sequence_num;
    database2[sequence_num] = *transData;
    if (transData->transactionSequenceNumber > 255) {
        SavetransCheck = SAVING_FAILED;
    }
    else {
        database2[transData->transactionSequenceNumber].cardHolderData = transData->cardHolderData;
        database2[transData->transactionSequenceNumber].terminalData = transData->terminalData;
        database2[transData->transactionSequenceNumber].transState = recieveTransCheck;
        database2[transData->transactionSequenceNumber].transactionSequenceNumber = transData->transactionSequenceNumber;
        sequence_num++;
        SavetransCheck = SERVER_OK;
    }
    return SavetransCheck;
}
///////////////////////////////////////////////////////////
EN_transStat_t recieveTransactionData(ST_transaction* transData) {
    if (isValidAccount(&transData->cardHolderData) == ACCOUNT_NOT_FOUND) {
        recieveTransCheck = ACCOUNT_INVALID;
    }
    if (isValidAccount(&transData->cardHolderData) == ACCOUNT_BLOCKED) {
        recieveTransCheck = DECLINED_STOLEN_CARD;
    }
    else if (isAmountAvailable(&transData->terminalData) == LOW_BALANCE) {
        recieveTransCheck = DECLINED_INSUFFECIENT_FUND;
    }
   /* else if (saveTransaction(&transData) == SAVING_FAILED) {
        recieveTransCheck = INTERNAL_SERVER_ERROR;
    }*/
    else {
        printf("Balance before transaction = %f\n", database1[accnum].balance);
        database1[accnum].balance = (database1[accnum].balance - transData->terminalData.transAmount);
        printf("Balance after transaction = %f \n", database1[accnum].balance);
        recieveTransCheck = APPROVED;
    }
    return recieveTransCheck;
}
//////////////////////////////////////////////////////////////////////////////
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction* transData) {
    uint32_t flag = 0;
    EN_serverError_t gettransCheck;
    for (int i = 0; i < sequence_num; i++) {
        if (transactionSequenceNumber == sequence_num) {
            flag = 1;
            break;
        }
        else {
            flag = 0;
        }
    }
    if (flag == 0) {
        printf("TRASACTION IS NOT FOUND IN THE BANK DATA BASE \n");
        gettransCheck = TRANSACTION_NOT_FOUND;
    }
    else {
        printf("TRASACTION IS FOUND IN THE BANK DATA BASE \n");
        printf("CARD HOLDER DATA :\n");
        fflush(stdout);
        printf("card holder name: %s \n", database2[transData->transactionSequenceNumber].cardHolderData.cardHolderName);
        fflush(stdout); fflush(stdin);
        printf("card expiry date: %s \n", database2[transData->transactionSequenceNumber].cardHolderData.cardExpirationDate);
        printf("card PAN :%s \n", database2[transData->transactionSequenceNumber].cardHolderData.primaryAccountNumber);
        printf("TERMINAL DATA :\n");
        printf("transaction date : %s", database2[transData->transactionSequenceNumber].terminalData.transactionDate);
        printf("maximum transaction : %f \n", database2[transData->transactionSequenceNumber].terminalData.maxTransAmount);
        printf("transaction amount :%f \n", database2[transData->transactionSequenceNumber].terminalData.transAmount);
        printf("transaction sequence number :%u \n", database2[transData->transactionSequenceNumber].transactionSequenceNumber);
        gettransCheck = SERVER_OK;

    }
    return gettransCheck;
}