"""
Filereader for putty .txt-generated datafiles
Plotters for dB and Polar

Made by: Anders Kristensen
"""

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.pylab as pl 

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
    if len(data) != 33:
        print(len(data))
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
        ax.set_title("Polar plot in delta", va='bottom')
        plt.show()

    ax.set_rmax(0)
    ax.set_rticks([-60, -50, -40, -30, -20])  # Less radial ticks
    ax.set_rlabel_position(-22.5)  # Move radial labels away from plotted line
    ax.grid(True,linewidth = 1)

    plt.show()


def plot_nulls(data: list) -> None:
    """Uses the matplotlib to plot nulls in polar form

        Parameters
        ----------
        data : str, list of data
        mode  : str, type of diagram
    """
    theta = []
    r = []
    nulls = data[0]
    measurements1m = data[1][0:10]
    measurements5m = data[1][10:20]
    measurements10m = data[1][20:30]

    inp = input("""
What distance do you want to plot?
(1) Degrees
(2) 5 meters
(3) Exit
Enter you option here: """)

    if inp == "1":
        fig, ax = plt.subplots(subplot_kw={'projection': 'polar'})
        for index, val in enumerate(measurements1m):
            x = int(val[0])-int(nulls[0][0])
            y = int(val[1])-int(nulls[0][1])
            theta.append(np.arctan2(y,x))
            r.append(np.sqrt(x**2+y**2))
        plt.polar(theta, r, 'b.', label="1 meter")
        r.clear()
        theta.clear()

        for index, val in enumerate(measurements5m):
            x = int(val[0])-int(nulls[1][0])
            y = int(val[1])-int(nulls[1][1])
            theta.append(np.arctan2(y,x))
            r.append(np.sqrt(x**2+y**2))
        plt.polar(theta, r, 'g.', label="5 meters")
        r.clear()
        theta.clear()

        for index, val in enumerate(measurements10m):
            x = int(val[0])-int(nulls[2][0])
            y = int(val[1])-int(nulls[2][1])
            theta.append(np.arctan2(y,x))
            r.append(np.sqrt(x**2+y**2))
        plt.polar(theta, r, 'r.', label="10 meters")
        ax.set_title("Nulls", va='bottom')
        ax.set_rlabel_position(-22.5)  # Move radial labels away from plotted line
        ax.grid(True,linewidth = 1)
        ax.legend(loc="upper right", bbox_to_anchor=(1.3, 1.05))

    elif inp == "2":
        xes = []
        ys = []
        for index, val in enumerate(measurements5m):
            if(int(val[0]) == 0):
                x = 0
            else:
                x = np.tan((90-int(val[0]))*np.pi/180)*5-0.52
            y = np.tan(int(val[1])*np.pi/180)*5-0.52
            xes.append(x)
            ys.append(y)
            print(x,y)
        
        print("here")
        plt.xlim(-1.25, 1,25)
        plt.ylim(-1.25, 1.25)
        plt.xticks(np.arange(-1.25, 1.5, 0.25))
        plt.yticks(np.arange(-1.25, 1.5, 0.25))
        plt.grid()
        plt.plot(xes,ys, marker="o", markersize=5, linestyle="", label="Nullpunkter funnet")
        plt.plot(0,0, marker="o", markersize=10,linestyle="" ,label="Referanse (nRF52832)")
        plt.xlabel("Avvik fra referansepunkt [m]")
        plt.ylabel("Avvik fra referansepunkt [m]")
        plt.legend(loc="upper left")
        plt.title("Resultat av fullskalatest ved 5m")

    elif inp == "3":
        return

    plt.show()





def plotprogram(data: list, mode: str) -> None:
    """The program that choses the type of diagram and loops the choices for
        the user.

        Parameters
        ----------
        data : str, list of data
        mode : str, mode of plot

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
    distance = input("is (i)n or (o)ut?: ")
    if distance == "i":
        path = "Other/Python/dataplotter/Inne/"
    elif distance == "o":
        m = input("meters? (1,5,10): ")
        path = "Other/Python/dataplotter/Ute/"+m+"meter/"
    file = input("Enter filename here: ")

    datafile = path+file

    try:
        with open(datafile, "r") as f: 
            type = input("Is this plot for (s)weeps or (n)ulls?: ")
            readings = filereader(f, type)
            print(len(readings))
            if type == "s":
                mode = input(instruction)
                plotprogram(readings, mode)
            else: plot_nulls(readings)
    except ValueError as e:
        print(e)


    