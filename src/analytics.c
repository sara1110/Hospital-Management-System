#include "include\analytics.h"

// Utility for calculating the amount
// of digits in an integer
int numPlaces (int n) {
    int r = 1;
    if (n < 0) n = (n == INT_MIN) ? INT_MAX: -n;
    while (n > 9) {
        n /= 10;
        r++;
    }
    return r;
}

// - Generates a BarChart out of a 4-char value series (plotted in the main axis)
// Examples of valid char values: MMMM, mllm J1, 11, 0830
// - heights[] should contain the height of each bar
// - nValues should tell both the length of series[] and heights[]
// - barDiff determines the difference (in chars) between bars in the chart
// - BarChart returned contains allocated arrays, so this should be later handled by
// freeBarChart() 
// - If an error occurred, returned BarChar will have a charHeight == -1
BarChart generateBarChart(char (series[])[LABELSTRSIZE], int heights[], int nValues, int barDiff) {
    
    BarChart outChart;
    outChart.charHeight=-1;
    if(NULL == series || NULL == heights || nValues<=0) 
        return outChart; // Invalid series

    // Allocate and copy arrays
    outChart.heights = (int*) malloc(sizeof(int) * nValues);
    memcpy(outChart.heights, heights, sizeof(int) * nValues);
    
    outChart.labels = calloc(LABELSTRSIZE*nValues, sizeof(char));
    memcpy(outChart.labels, series, sizeof(char) * LABELSTRSIZE * nValues);

    // If provided barDiff is not valid, use the default one
    outChart.charBarDiff = (barDiff > 0) ?  barDiff : DEFBARDIFF;

    outChart.nValues = nValues;

    outChart.charHeight = nValues + ((nValues-1) * barDiff);
    outChart.charWidth = DEFCHARTWIDTH;

    return outChart;
}

// Prints a given BarChart to the terminal screen
void printBarChart(BarChart chart) {

    // horizontal offset of the chart
    const char* offset = "     ";

    // Linear search for the highest value in the heights
    int highest = chart.heights[0];
    for(int h=0; h < chart.nValues; h++) {
        if(chart.heights[h] > highest) 
            highest = chart.heights[h];
    }

    // Calculate the size of a unit as plotted in the chart (scale)
    int scaleUnitSize = (chart.charWidth-RIGHTCHARTPADDING) / highest;

    // Print the top horizontal axis
    {
        printf(offset);
        for(int i = 0; i < chart.charWidth; i++) {
            printf("_");
        }
        printf("\n");
    }

    // Print the vertical axis 
    // with the bars
    {
        int barDiffCounter = 0;
        int valueCounter = 0;
        for(int i = 0; i <= chart.charHeight + 1; i++) {
            
            if(barDiffCounter < chart.charBarDiff) {
                // Increment barDiffCounter
                printf(offset);
                printf("|");
                barDiffCounter++;
            }
            else {

                setCMDcolor(ANSI_YELLOW);
                printf(chart.labels[valueCounter]);
                resetCMDcolor();
                printf(" |");
                if(valueCounter <= chart.nValues) {
                    // Print bar for this value
                    for(int j=0; j< chart.heights[valueCounter]*scaleUnitSize; j++) {
                        printf("=");
                    }
                    
                    printf(" %d", chart.heights[valueCounter]);
                    valueCounter++;
                }

                // Reset barDiffCounter
                barDiffCounter=0;
            }
            printf("\n");
        }
    }
}

// Deallocates the arrays in a chart
void freeBarChart(BarChart *chart) {
    if(NULL!=chart->heights)
        free(chart->heights);
    if(NULL!=chart->labels)
        free(chart->labels);
}
