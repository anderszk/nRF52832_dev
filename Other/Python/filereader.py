"""
Filereader for putty .txt-generated datafiles
Plotters for dB and Polar

Made by: Anders Kristensen
"""

import os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.pylab as pl 

#path = r"C:\Users\khuon\Documents\logs\"
instruction = """
Press one of the following numbers to plot a chart:\n
(1) dB Sigma
(2) dB Delta
(3) Polar Sigma
(4) Polar Delta
(5) To exit

Enter your choice (1-5) here: """





def read_data_sweep(file) -> list:
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


def read_data_nulls(file) -> list:
    """readsnull

        Parameters
        ----------
        file : file, datafile

        Returns
        ----------
        data : list, list of data
    """
    data = []
    for line in file:
        data.append(line.strip("\n").split(","))
    if len(data) != 18:
        raise IndexError("Invalid data in list, please upload the file again")
    return [data[0:3],data[3:]]





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

def filereader(file, type):
    if type in ["s","S"]:
        return read_data_sweep(file)
    elif type in ["n","N"]:
        return read_data_nulls(file)
    else: 
        print("invalid mode, please try again")
        filereader(file)





if __name__ == "__main__":
    #writedatatofile()
    #datafile = path+input("Enter filename here (eg. file.txt): ")
    #with open(os.path.join(os.path.dirname(__file__), datafile), 'r') as f:

    file = input("Enter filename here: ")
    datafile = "logs/"+file
    try:
        with open(datafile, "r") as f: #temp
            type = input("Is this plot for (s)weeps or (n)ulls?: ")
            readings = filereader(f, type)
            print(len(readings))
            if type == "s":
                mode = input(instruction)
                plotprogram(readings, mode)
            else: print(readings)
    except ValueError as e:
        print(e)


    