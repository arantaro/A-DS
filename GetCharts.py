import matplotlib.pyplot as plt
import csv

from math import asin, pi


def get_cords(filename, precise_area):
    points_axis = []
    area_axis = []
    deviation_axis = []
    with open(filename, "r") as datafile:
        rows = csv.reader(datafile, delimiter=' ')

        for row in rows:
            points_axis.append(float(row[0]))
            area_axis.append(float(row[1]))
            deviation_axis.append(abs(float(row[1]) - precise_area) / precise_area)
    return points_axis, area_axis, deviation_axis


if __name__ == '__main__':
    precise_area = 0.25 * pi + 1.25 * asin(0.8) - 1

    for i in range(10):
        for folder_name in ["narrowAreaData", "wideAreaData"]:
            filename = folder_name + "/" + str(i + 1) + ".csv"
            points_axis, area_axis, deviation_axis = get_cords(filename, precise_area)
            plt.subplot(2, 1, 1)
            plt.plot(points_axis, area_axis)
            plt.xlabel("Points")
            plt.ylabel("Intersection area")
            plt.title(filename)

            plt.subplot(2, 1, 2)
            plt.plot(points_axis, deviation_axis)
            plt.xlabel("Points")
            plt.ylabel("Relative deviation")
            plt.title(filename)

            plt.tight_layout()
            plt.savefig(folder_name + "/" + str(i + 1) + ".png")
            plt.clf()
