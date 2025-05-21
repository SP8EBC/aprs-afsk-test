#include <cstdint>
#include <cstdio>
#include <array>
#include <algorithm>
#include <cmath>
#include <iostream>

#include <sciplot/sciplot.hpp>

#include "correlation.h"

#define SAMPLES_BUF_LN (SAMPLERATE * 4)


#define SAMPLE_TO_RADIAN(x)  ((x * M_PI) / (static_cast<double>(SAMPLERATE) / 2.0)) 

std::array<double, SAMPLES_BUF_LN> samples;

std::array<double, SAMPLES_BUF_LN> mark_i;
std::array<double, SAMPLES_BUF_LN> mark_q;
std::array<double, SAMPLES_BUF_LN> space_i;
std::array<double, SAMPLES_BUF_LN> space_q;

std::array<double, SAMPLES_BUF_LN> results;


int main (int argc, char *argv[]) {
    int i = 0;

    // test frequency
    double frequency = 1200;

    uint32_t xAxisEnd = SAMPLES_BUF_LN / frequency * 2.0;

    if (frequency < 2.0) {
        xAxisEnd = SAMPLES_BUF_LN;
    }

    if (argc >= 2) {
        frequency = static_cast<double>(::atof(argv[1]));
        std::cout << "user selected frequency: " << frequency << "Hz" << std::endl;
    }

    std::for_each(samples.begin(), samples.end(), [&i, frequency](double& x ) {x = std::sin(frequency * SAMPLE_TO_RADIAN(i++));});

    ::afsk_init();

    auto markI = mark_i.begin();
    auto markQ = mark_q.begin();
    auto spaceI = space_i.begin();
    auto spaceQ = space_q.begin();

    auto resultIt = results.begin();

    for (double sample : samples) {
        std::tuple<double, double, double, double> result = ::afsk_demod(sample);
        *markI = std::get<0, double>(result);
        *markQ = std::get<1, double>(result);
        *spaceI = std::get<2, double>(result);
        *spaceQ = std::get<3, double>(result);

        const double r =  (*spaceI)*(*spaceI)+
                    (*spaceQ)*(*spaceQ)-
                    (*markI)*(*markI)-
                    (*markQ)*(*markQ);

        *resultIt = r;

        markI++;
        markQ++;
        spaceI++;
        spaceQ++;
        resultIt++;
    }

    // Create values for your x-axis
    sciplot::Vec x = sciplot::linspace(0.0, static_cast<double>(SAMPLES_BUF_LN), SAMPLES_BUF_LN);

    // Create a Plot object
    sciplot::Plot2D plot1, plot2;
    // Set color palette for first Plot
    plot1.palette("paired");
    // // Draw a cosine graph putting x on the x-axis and cos(x) on the y-axis
    plot1.drawCurve(x, mark_i).label("markI").lineWidth(2).lineColor("red");
    plot1.drawCurve(x, mark_q).label("markQ").lineWidth(2).lineColor("green");
    plot1.drawCurve(x, space_i).label("spaceI").lineWidth(2).lineColor("blue");
    plot1.drawCurve(x, space_q).label("spaceQ").lineWidth(2).lineColor("black");

    plot1.xrange(0.0, static_cast<double>(xAxisEnd));
    plot1.xtics().increment(xAxisEnd / 4);
    plot1.xticsMajorBottom().alongAxis();
    plot1.grid().lineWidth(1).back().show();
    plot1.legend().fontSize(14).atOutsideBottom().displayVertical();

    plot2.palette("paired");
    plot2.drawCurve(x, samples).label("samples").lineWidth(2).lineColor("red");
    plot2.drawCurve(x, results).label("results").lineWidth(5).lineColor("orange");
    plot2.legend().atOutsideBottom().displayVertical();

    // Put both plots in a "figure" horizontally next to each other
    sciplot::Figure figure = {{plot1, plot2}};

    // Create a canvas / drawing area to hold figure and plots
    sciplot::Canvas canvas = {{figure}};

    // Set color palette for all Plots that do not have a palette set (plot2) / the default palette
    canvas.defaultPalette("set1");

    canvas.size(1600, 900);

    // Show the canvas in a pop-up window
    canvas.show();

    // Save the plot to a SVG file
    canvas.save("example-readme.svg");

    return 0;
}