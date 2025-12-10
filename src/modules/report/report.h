#ifndef REPORT_H
#define REPORT_H

#include "../../structs.h"

void menuReporting();

void dailySalesReport();
void shiftReport();
void productSalesReport();

void stockReport();
void lowStockReport();

void calculateDailySales(const char *date, double *totalSales, int *totalTransactions);
void getMostSoldProducts(int topN);

#endif