/*
 * plot.ino - Handles plotting signal data
 * via Devin Conley's Arduino Plotter library.
 */

#include <Plotter.h>
Plotter plot;

double t0PlotRunningMean;
double t0plot_data;
double t0PlotMinPos;
double t0PlotMaxPos;
double t0PlotMinNeg;
double t0PlotMaxNeg;
double t1PlotRunningMean;
double t1plot_data;
double t1PlotMinPos;
double t1PlotMaxPos;
double t1PlotMinNeg;
double t1PlotMaxNeg;

void setup_plot() {
  plot.Begin();
  plot.AddTimeGraph("T0 Data", 200, "Data", t0plot_data, "Running Mean", t0PlotRunningMean, "Min", t0PlotMinPos, "Max", t0PlotMaxPos, "Min", t0PlotMinNeg, "Max", t0PlotMaxNeg);
  plot.AddTimeGraph("T1 Data", 200, "Data", t1plot_data, "Running Mean", t1PlotRunningMean, "Min", t1PlotMinPos, "Max", t1PlotMaxPos, "Min", t1PlotMinNeg, "Max", t1PlotMaxNeg);
}

void plot_data() {
    t0plot_data = thermometer_0_data.get_latest_data();
    t0PlotRunningMean = thermometer_0_data.get_running_mean();
    t0PlotMinPos = t0PlotRunningMean + thermometer_0_data.min_activation;
    t0PlotMaxPos = t0PlotRunningMean + thermometer_0_data.max_activation;
    t0PlotMinNeg = t0PlotRunningMean - thermometer_0_data.min_activation;
    t0PlotMaxNeg = t0PlotRunningMean - thermometer_0_data.max_activation;
 
    t1plot_data = thermometer_1_data.get_latest_data();
    t1PlotRunningMean = thermometer_1_data.get_running_mean();
    t1PlotMinPos = t1PlotRunningMean + thermometer_1_data.min_activation;
    t1PlotMaxPos = t1PlotRunningMean + thermometer_1_data.max_activation;
    t1PlotMinNeg = t1PlotRunningMean - thermometer_1_data.min_activation;
    t1PlotMaxNeg = t1PlotRunningMean - thermometer_1_data.max_activation;
 
    plot.Plot();
}
