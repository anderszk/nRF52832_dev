"""
Filereader for putty .txt-generated datafiles
Plotters for dB and Polar

Made by: Anders Kristensen
"""

import os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.pylab as pl 

datafile = "file1.txt" #temp
#path = r"C:\Users\khuon\Documents\logs\"
instruction = """
Press one of the following numbers to plot a chart:\n
(1) dB Sigma
(2) dB Delta
(3) Polar Sigma
(4) Polar Delta
(5) To exit

Enter your choice (1-4) here: """



def writedatatofile() -> None:
    with open(datafile, "w") as f:
        for i in range(0,90):
            string = "{},{},{}\n".format(i, -20-i ,i) 
            f.write(str(string))
        for i in range(90,180):
            string = "{},{},{}\n".format(i, -160+i, i)
            f.write(str(string))


def read_data(file) -> list:
    """Reads the data from the files and returns a list containing of 3 sub-lists.

        Parameters
        ----------
        file : file
            File containing all the data, comma-seperated

        Returns
        ------
        list:
            list of all the data.
    """
    encoder = []
    sigma = []
    delta = []
    for line in file:
        data = line.split(',')
        encoder.append(float(data[0]))
        sigma.append(float(data[1]))
        delta.append(float(data[2]))
    return [encoder, sigma, delta]


def plot_db(data: list, mode: str) -> None:
    """Uses the matplotlib to plot a dB-diagram

        Parameters
        ----------
        data : str, list of data
        mode  : str, type of diagram
    """
    if mode == "1":
        plt.plot(data[0], data[1])
    elif mode == "2":
        plt.plot(data[0], data[2])
    plt.xticks = np.arange(min(data[0]), max(data[0])+1, 5.0)
    plt.ylabel('RSSI [dBm]')
    plt.xlabel('Angle [degrees]')
    plt.show()


def plot_polar(data: list, mode: str) -> None:
    """Uses the matplotlib to plot a polar diagram

        Parameters
        ----------
        data : str, list of data
        mode  : str, type of diagram
    """
    fig, ax = plt.subplots(subplot_kw={'projection': 'polar'})

    if mode == "3":
        # r = np.arange(data[1], 0.1)
        # theta = 2 * np.pi * r
        # ax.plot(theta, data[1])
        for index, val in enumerate(data[1]):
            plt.polar(np.pi*index/180, data[1][index], 'r.')
        ax.set_title("Polar plot in sigma", va='bottom')
        plt.show()

    elif mode == "4":
        for index, val in enumerate(data[2]):
            plt.polar(np.pi*index/180, data[2][index], 'b.')
        ax.set_title("Polar plot in sigma", va='bottom')
        plt.show()

    ax.set_rmax(0)
    ax.set_rticks([-60, -50, -40, -30, -20])  # Less radial ticks
    ax.set_rlabel_position(-22.5)  # Move radial labels away from plotted line
    ax.grid(True,linewidth = 1)

    plt.show()


def plotprogram(data: list, mode: str) -> None:
    """The program that choses the type of diagram and loops the choices for
        the user.

        Parameters
        ----------
        data : str, list of data
        mode  : str, type of diagram 

        Raises
        ------
        ValueError:
            If the value netered are outside the range of the choices, it will
            raise an ValueError to let the user know it is an invalid choice.
    """
    if mode in ["1","2"]:
        print("\nPlotting dB1 diagram: ")
        plot_db(data, mode)
        plotprogram(data, input(instruction))
    elif mode in ["3","4"]:
        print("\nPlotting polar diagram: ")
        plot_polar(data, mode)
        plotprogram(data, input(instruction))
    elif mode in ["5"]:
        return
    else:
        raise ValueError("Invalid type, please try with a number between 1-5")


""" 
TODO:
    -Legge til funksjon som plotter nullpunktene
"""


if __name__ == "__main__":
    #writedatatofile()
    #datafile = path+input("Enter filename here (eg. file.txt): ")
    #with open(os.path.join(os.path.dirname(__file__), datafile), 'r') as f:
    with open(datafile, "r") as f: #temp
        readings = read_data(f)
        try:
            mode = input(instruction)
            plotprogram(readings, mode)
        except ValueError as e:
            print(e)

    