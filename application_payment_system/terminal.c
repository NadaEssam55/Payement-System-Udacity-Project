#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#include <stdio.h>
#include "terminal.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TRANSACTION_AMOUNT 5000

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {
	EN_terminalError_t TrasDateCheck;
	time_t t;
	t = time(NULL);
	DATE info = *localtime(&t);
	sprintf(termData->transactionDate, "%02d/%02d/%02d\n", info.tm_mday, (info.tm_mon + 1), (info.tm_year + 1900));
	size_t length = strlen(termData->transactionDate);
	if (length == NULL) {
		printf("WRONG TRANSACTION DATE FORMATE !");
		TrasDateCheck = WRONG_DATE;
	}
	else if (!(isdigit(termData->transactionDate[0])) || !(isdigit(termData->transactionDate[1]))) {
		printf("WRONG TRANSACTION DATE FORMATE !");
		TrasDateCheck = WRONG_DATE;
	}
	else if ((termData->transactionDate[2]) != '/') {
		printf("WRONG TRANSACTION DATE FORMATE !");
		TrasDateCheck = WRONG_DATE;
	}
	else if (!(isdigit(termData->transactionDate[3])) || !(isdigit(termData->transactionDate[4]))) {
		printf("WRONG TRANSACTION DATE FORMATE !");
		TrasDateCheck = WRONG_DATE;
	}
	else if ((termData->transactionDate[5]) != '/') {
		printf("WRONG TRANSACTION DATE FORMATE !");
		TrasDateCheck = WRONG_DATE;
	}
	else if (!(isdigit(termData->transactionDate[6])) || !(isdigit(termData->transactionDate[7]))) {
		printf("WRONG TRANSACTION DATE FORMATE !");
		TrasDateCheck = WRONG_DATE;
	}
	else if (!(isdigit(termData->transactionDate[8])) || !(isdigit(termData->transactionDate[9]))) {
		printf("WRONG TRANSACTION DATE FORMATE !");
		TrasDateCheck = WRONG_DATE;
	}
	else {
		printf("The transaction date is %s", termData->transactionDate);
		TrasDateCheck = TERMINAL_OK;
	}
	return TrasDateCheck;
}
////////////////////////////////////////////////////////////////////////
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData) {
	EN_terminalError_t ExpierdcardCheck;
	if (cardData->cardExpirationDate[3] < termData->transactionDate[8]) {
		printf("DECLINED EXIRED CARD \n");
		ExpierdcardCheck = EXPIRED_CARD;
	}
	else if (cardData->cardExpirationDate[3] == termData->transactionDate[8]) {
		if (cardData->cardExpirationDate[4] < termData->transactionDate[9]) {
			printf("DECLINED EXIRED CARD \n");
			ExpierdcardCheck = EXPIRED_CARD;
		}
		else if (cardData->cardExpirationDate[4] == termData->transactionDate[9]) {
			if (cardData->cardExpirationDate[0] < termData->transactionDate[3]) {
				printf("DECLINED EXIRED CARD \n");
				ExpierdcardCheck = EXPIRED_CARD;
			}
			else if (cardData->cardExpirationDate[0] == termData->transactionDate[3]) {
				if (cardData->cardExpirationDate[1] < termData->transactionDate[4]) {
					printf("DECLINED EXIRED CARD \n");
					ExpierdcardCheck = EXPIRED_CARD;
				}
				else {
					printf("CARD IS ACCEPTED \n");
					ExpierdcardCheck = TERMINAL_OK;
				}
			}
			else {
				printf("CARD IS ACCEPTED \n");
				ExpierdcardCheck = TERMINAL_OK;
			}
		}
		else {
			printf("CARD IS ACCEPTED \n");
			ExpierdcardCheck = TERMINAL_OK;
		}
	}
	else {
		printf("CARD IS ACCEPTED \n");
		ExpierdcardCheck = TERMINAL_OK;
	}
	return ExpierdcardCheck;
}
/////////////////////////////////////////////////////////////////////
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
	EN_terminalError_t TansamountCheck;
	printf("Enter the transaction amount:");
	scanf("%f", &termData->transAmount);
	if (termData->transAmount <= 0.0) {
		printf("INVALID AMOUNT");
		TansamountCheck = INVALID_AMOUNT;
	}
	else {
		TansamountCheck = TERMINAL_OK;
	}
	return TansamountCheck;
}
//////////////////////////////////////////////////////////////////////
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData) {
	EN_terminalError_t MaxamountCheck;
	termData->maxTransAmount = MAX_TRANSACTION_AMOUNT;
	printf("Max transaction amount is %f\n", termData->maxTransAmount);
	if (termData->maxTransAmount <= 0) {
		MaxamountCheck = INVALID_MAX_AMOUNT;
	}
	else {
		MaxamountCheck = TERMINAL_OK;
	}
	return MaxamountCheck;
}
///////////////////////////////////////////////////////////////////////
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {
	EN_terminalError_t MaxtransCheck;
	if (termData->transAmount > termData->maxTransAmount) {
		printf("DECLINED AMOUNT EXCEEDING LIMIT");
		MaxtransCheck = EXCEED_MAX_AMOUNT;
	}
	else {
		MaxtransCheck = TERMINAL_OK;
	}
	return MaxtransCheck;
}
