#include <cstdint>
#include <cstdio>
#include <array>
#include <algorithm>
#include <cmath>

#include <sciplot/sciplot.hpp>

#define SAMPLES_BUF_LN 38400


#define SAMPLE_TO_RADIAN(x)  ((x * M_PI) / (static_cast<double>(SAMPLERATE) / 2.0)) 

int main (int argc, char *argv[]) {
    int i = 0;
    // Create values for your x-axis
    sciplot::Vec x = sciplot::linspace(0.0, static_cast<double>(SAMPLES_BUF_LN), SAMPLES_BUF_LN);

    std::array<double, SAMPLES_BUF_LN> samples;
    std::for_each(samples.begin(), samples.end(), [&i](double& x ) {x = std::sin(SAMPLE_TO_RADIAN(i++));});

    // Create a Plot object
    sciplot::Plot2D plot1;
    // Set color palette for first Plot
    plot1.palette("paired");
    // Draw a sine graph putting x on the x-axis and sin(x) on the y-axis
    plot1.drawCurve(x, samples).label("samples").lineWidth(2);
    // // Draw a cosine graph putting x on the x-axis and cos(x) on the y-axis
    // plot1.drawCurve(x, std::cos(x)).label("cos(x)").lineWidth(2);

    // Put both plots in a "figure" horizontally next to each other
    sciplot::Figure figure = {{plot1}};

    // Create a canvas / drawing area to hold figure and plots
    sciplot::Canvas canvas = {{figure}};

    // Set color palette for all Plots that do not have a palette set (plot2) / the default palette
    canvas.defaultPalette("set1");

    // Show the canvas in a pop-up window
    canvas.show();

    // Save the plot to a SVG file
    canvas.save("example-readme.svg");

    return 0;
}