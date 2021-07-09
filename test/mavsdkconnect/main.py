import sys
import datetime
import asyncio
from mavsdk import System
from pymongo import MongoClient

information = ''

async def run():
    # Init the drone
    drone = System()
    await drone.connect(system_address="serial:///dev/ttyACM0:115200")
    client = MongoClient('localhost', 27017)

    db = client["UAV"]
    battery = db["Battery"]
    gps = db["GPS"]
    in_air = db["IN_AIR"]
    position = db["Position"]

    # Start the tasks
    asyncio.ensure_future(print_battery(drone, battery))
    asyncio.ensure_future(print_gps_info(drone, gps))
    asyncio.ensure_future(print_in_air(drone, in_air))
    asyncio.ensure_future(print_position(drone, position))


async def print_battery(drone, collection):
    async for battery in drone.telemetry.battery():
        print(f"Battery: {battery.remaining_percent}")
        battery_ = {
            "Battery" : battery.remaining_percent,
            "Date": str(datetime.datetime.now().date()),
            "Hour": str(datetime.datetime.now().hour),
            "Minute": str(datetime.datetime.now().minute)
        }
        result = collection.insert_one(battery_)


async def print_gps_info(drone, collection):
    async for gps_info in drone.telemetry.gps_info():
        print(f"GPS info: {gps_info}")
        gps_ = {
            "GPS": str(gps_info),
            "Date": str(datetime.datetime.now().date()),
            "Hour": str(datetime.datetime.now().hour),
            "Minute": str(datetime.datetime.now().minute)
        }
        result = collection.insert_one(gps_)


async def print_in_air(drone, collection):
    async for in_air in drone.telemetry.in_air():
        print(f"In air: {in_air}")
        in_air_ = {
            "In Air" : in_air,
            "Date": str(datetime.datetime.now().date()),
            "Hour": str(datetime.datetime.now().hour),
            "Minute": str(datetime.datetime.now().minute)
        }
        result = collection.insert_one(in_air_)


async def print_position(drone, collection):
    async for position in drone.telemetry.position():
        print(position)
        position_ = {
            "Position" : str(position),
            "Date": str(datetime.datetime.now().date()),
            "Hour": str(datetime.datetime.now().hour),
            "Minute": str(datetime.datetime.now().minute)
        }
        result = collection.insert_one(position_)

if __name__ == "__main__":
    # Start the main function
    asyncio.ensure_future(run())

    # Runs the event loop until the program is canceled with e.g. CTRL-C
    asyncio.get_event_loop().run_forever()

