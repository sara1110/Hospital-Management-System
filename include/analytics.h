#pragma once
#ifndef ANALYTICS_H
#define ANALYTICS_H

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "include\cmdui_utils.h"

#define DEFBARDIFF 1
#define DEFCHARTWIDTH 50
#define RIGHTCHARTPADDING 5
#define LABELSTRSIZE 5 // 4 chars + 1 zero char
#define BARCHAR '='

#define INT_MAX  (2147483647)
#define INT_MIN  (-2147483647)
// Utility for calculating the amount
// of digits in an integer
int numPlaces (int n);

// Represents a Bar Chart extending downwards
// (bars extend from left to right)
typedef struct barChartStrc {
    int charHeight; // height of the chart in chars (up to down)
    int charWidth; // width of the chart in charts (left to right)
    int charBarDiff; // char difference between bars
    char (*labels)[LABELSTRSIZE]; // series values (plotted in the vertical axis)
    int *heights; // dependent values for the series (plotted on the horizontal axis)
    int nValues;
} BarChart;

// - Generates a BarChart out of a 4-char value series (plotted in the main axis)
// Examples of valid char values: MMMM, mllm J1, 11, 0830
// - heights[] should contain the height of each bar
// - nValues should tell both the length of series[] and heights[]
// - barDiff determines the difference (in chars) between bars in the chart
// - BarChart returned contains allocated arrays, so this should be later handled by
// freeBarChart() 
// - If an error occurred, returned BarChar will have a charHeight == -1
BarChart generateBarChart(char (labels[])[LABELSTRSIZE], int heights[], int nValues, int barDiff);

// Prints a given BarChart to the terminal screen
void printBarChart(BarChart chart);

// Deallocates the arrays in a chart
void freeBarChart(BarChart *chart);

#endif // ANALYTICS_H