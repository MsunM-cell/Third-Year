
from os import dup
import random
import pandas as pd
from sklearn.cluster import KMeans
from sklearn.cluster import DBSCAN
import matplotlib.pyplot as plt
inputfile = './lngstationOutput.csv'
outputfile = './temp.txt'
#latitude longitude code
data = pd.read_csv(inputfile,sep=' ')
data=data[['code','latitude','longitude']]
data['isLNG']="true"
data['IN']="true"
data.to_json(open(outputfile,'w'),orient='records')