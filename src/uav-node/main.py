import sys
import datetime
import asyncio
from mavsdk import System
import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish

async def run():
    # Init the drone
    drone = System()
    await drone.connect(system_address="serial:///dev/ttyACM0:115200")

    # Start the tasks
    asyncio.ensure_future(print_battery(drone))
    asyncio.ensure_future(print_gps_info(drone))
    asyncio.ensure_future(print_in_air(drone))
    asyncio.ensure_future(print_position(drone))


async def print_battery(drone):
    async for battery in drone.telemetry.battery():
    #    print(f"Battery: {battery.remaining_percent}")
        battery_ = "{" + "\"battery\" : " + "\"" + str(battery.remaining_percent) + "\"" + "}"
        #    "Date": str(datetime.datetime.now().date()),
        #    "Hour": str(datetime.datetime.now().hour),
        #    "Minute": str(datetime.datetime.now().minute)

        publish.single("UAV/Battery", str(battery_), hostname="citlab.myftp.org")

async def print_gps_info(drone):
    async for gps_info in drone.telemetry.gps_info():
    #    print(f"GPS info: {gps_info}")
        gps_ = "{" + "\"gps\" : " + "\"" + str(gps_info) + "\"" + "}"
           # "Date": str(datetime.datetime.now().date()),
           # "Hour": str(datetime.datetime.now().hour),
           # "Minute": str(datetime.datetime.now().minute)

        publish.single("UAV/GPS", str(gps_), hostname="citlab.myftp.org")

async def print_in_air(drone):
    async for in_air in drone.telemetry.in_air():
    #    print(f"In air: {in_air}")
        in_air_ = "{" + "\"inAir\" : " + "\"" + str(in_air) + "\"" + "}"
#            "In-Air" : in_air,
#            "Date": str(datetime.datetime.now().date()),
#            "Hour": str(datetime.datetime.now().hour),
#            "Minute": str(datetime.datetime.now().minute)
        publish.single("UAV/In-Air", str(in_air_), hostname="citlab.myftp.org")

async def print_position(drone):
    async for position in drone.telemetry.attitude_angular_velocity_body():
        #print(position.pitch_rad_s)
        pitch = position.pitch_rad_s
        yall = position.yaw_rad_s
        roll = position.roll_rad_s
        position_ = "{" + "\"Pitch\" : " + "\"" + str(pitch) + "\"" + "," +  "\"Yall\" : " + "\"" + str(yall) + "\"" + ","+ "\"Roll\" : " + "\"" + str(pitch) + "\"" +"}"
#            "Position" : str(position),
#            "Date": str(datetime.datetime.now().date()),
#            "Hour": str(datetime.datetime.now().hour),
#            "Minute": str(datetime.datetime.now().minute)
#        }
        print(position_)
        publish.single("UAV/Position", str(position_), hostname="citlab.myftp.org")

# Press the green button in the gutter to run the script.
if __name__ == "__main__":
    # Start the main function
    asyncio.ensure_future(run())

    # Runs the event loop until the program is canceled with e.g. CTRL-C
    asyncio.get_event_loop().run_forever()
