import sys
import datetime
from pymongo import MongoClient
from random import seed
from random import randint
seed(1)

class MongoDBConnect():
    def __init__(self):
        self.setupMongodb()

    def setupMongodb(self):
        self.client = MongoClient('localhost', 27017)
        self.db = self.client["LoRaNetwork"]
        self.weatherStation = self.db["WeatherStationDay"]
        self.airLight = self.db["Air-Light-Day"]

        for x in self.weatherStation.find():
            print("Weather Station", x)
        for x in self.airLight.find():
            print("Air Light", x)

    def create_weather_data(self, date, hour, minute):
        print(date)

        self.windDirection = 90 + randint(0, 5)
        self.windSpeedMax = randint(3, 5)
        self.windSpeedAverage = randint(0, 5)
        self.temperature = 25 + randint(0, 5)
        self.rainFallOneHour = 0
        self.rainFallOneDay = 0
        self.humidity = 40 + randint(10, 20)
        self.barPressure = 101 + randint(0, 1)

        data = {
            "WindDirection": self.windDirection,
            "WindSpeedAverage": self.windSpeedAverage,
            "WindSpeedMax": self.windSpeedMax,
            "Temperature": self.temperature,
            "RainfallOneHour": self.rainFallOneHour,
            "RainfallOneDay": self.rainFallOneDay,
            "Humidity": self.humidity,
            "BarPressure": self.barPressure,
            "Date": date,
        }
        print(data)
        self.result = self.weatherStation.insert_one(data)

    def create_air_data(self, date, hour, minute):
        print(date)

        self.vis = randint(0, 100)
        self.ir = randint(0, 100)
        self.vr = randint(0, 100)
        self.co = randint(0, 100)
        self.nh3 = randint(0, 100)
        self.no2 = randint(0, 100)

        data = {
            "Vis": self.vis,
            "IR": self.ir,
            "VR": self.vr,
            "CO": self.co,
            "NH3": self.nh3,
            "NO2": self.no2,
            "Date": date,
        }
        print(data)
        self.result = self.airLight.insert_one(data)

if __name__ == "__main__":
    test = MongoDBConnect()
    for day in range(1, 32):
        date = str(day)+ "/10/2020"
        test.create_weather_data(date, 0, 0)
        test.create_air_data(date, 0, 0)
        
  	
    for day in range(1, 31):
        date = str(day) + "/11/2020"
        test.create_weather_data(date, 0, 0)
        test.create_air_data(date, 0, 0)
     		
     
    print("done")
