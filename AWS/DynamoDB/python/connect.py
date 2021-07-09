from pymongo import MongoClient
import json
import pprint
import boto3
import random

def create_item_table(dynamodb = None):
    if not dynamodb:
        dynamodb = boto3.resource('dynamodb', region_name='ap-southeast-1')

    table = dynamodb.create_table(
        TableName='Item',
        KeySchema=[
            {
                'AttributeName': 'Id',
                'KeyType': 'HASH'  # Partition key
            },
            {
                'AttributeName': 'item',
                'KeyType': 'RANGE'  # Sort key
            }
        ],
        AttributeDefinitions=[
            {
                'AttributeName': 'Id',
                'AttributeType': 'N'
            },
            {
                'AttributeName': 'item',
                'AttributeType': 'S'
            },

        ],
        ProvisionedThroughput={
            'ReadCapacityUnits': 5,
            'WriteCapacityUnits': 5
        }
    )
    return table

def load_item(item, dynamodb=None):
    if not dynamodb:
        dynamodb = boto3.resource('dynamodb', region_name='ap-southeast-1')
    
    table = dynamodb.Table('WeatherStation')
    
#    id_ = item['Name']
#    item_ = item['Age']

#    print("Adding item:", id_, item_)
#    item = json.dumps(item) 
    table.put_item(Item = item) 


client = MongoClient()
db = client.mydatabase
collection = db.customers

#for item in collection.find():
#    pprint.pprint(item)

if __name__ == '__main__':
#    item_table = create_item_table()
#    print("Table status:", item_table.table_status)

    dynamodb = boto3.resource('dynamodb', region_name='ap-southeast-1')
    
    table = dynamodb.Table('weatherStation')

    id = 0

    for id in range(0,200): 
        weather = {
                "Temperature" : random.randint(30, 70),
                "Humidity" : random.randint(10, 90)
            }
        data =  {
            "id" : id,
            "data" : str(weather)
        }
        pprint.pprint(data)
    #    data = json.dumps(data)
    #    data = json.loads(data['id'][0]['data'])
        table.put_item(Item = data) 






