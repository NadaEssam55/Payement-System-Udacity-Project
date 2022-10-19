#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED
#include<stdio.h>
#include "card.h"
#include"terminal.h"
#define MAX_ACCOUNTS_NUMBER 255
typedef unsigned int uint32_t;

typedef enum EN_transState_t
{
	APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, INTERNAL_SERVER_ERROR,ACCOUNT_INVALID
}EN_transStat_t;

typedef struct ST_transaction_t
{
	ST_cardData_t cardHolderData;
	ST_terminalData_t terminalData;
	EN_transStat_t transState;
	uint32_t transactionSequenceNumber;
}ST_transaction;


typedef enum EN_serverError_t
{
	SERVER_OK, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE, ACCOUNT_BLOCKED
}EN_serverError_t;

typedef enum {
	BLOCKED,
	RUNNING
}ACCOUNT_STATUS;

typedef struct ST_accountsDB_t
{
	float balance;
	uint8_t primaryAccountNumber[20];
	ACCOUNT_STATUS state;
}ST_accountsDB_t;

EN_transStat_t recieveTransactionData(ST_transaction* transData);
EN_serverError_t isValidAccount(ST_cardData_t* cardData);
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData);
EN_serverError_t saveTransaction(ST_transaction* transData);
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction* transData);
#endif
