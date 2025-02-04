import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sb
import string
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import classification_report, accuracy_score, confusion_matrix
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import roc_auc_score
from sklearn.metrics import roc_curve, auc
import missingno as msno
import sys

df=pd.read_csv(r"Titanic-Dataset.csv")

df.info()

df.isnull().sum()

s = sb.countplot(x = 'Pclass',data = df) #celci pour affichage de diagramme
sizes=[]
for p in s.patches:
    height = p.get_height()
    sizes.append(height)
    s.text(p.get_x()+p.get_width()/2,height +5,'{:1.2f}%'.format(height/len(df)*100), ha="center",fontsize=14)

s = sb.countplot(x = 'Survived',data = df) #celci pour affichage de diagramme
sizes=[]
for p in s.patches:
    height = p.get_height()
    sizes.append(height)
    s.text(p.get_x()+p.get_width()/2,height +5,'{:1.2f}%'.format(height/len(df)*100), ha="center",fontsize=14)

s = sb.countplot(x = 'Sex',data = df) #celci pour affichage de diagramme
sizes=[]
for p in s.patches:
    height = p.get_height()
    sizes.append(height)
    s.text(p.get_x()+p.get_width()/2,height +5,'{:1.2f}%'.format(height/len(df)*100), ha="center",fontsize=14)

s = sb.countplot(x = 'SibSp',data = df) #celci pour affichage de diagramme
sizes=[]
for p in s.patches:
    height = p.get_height()
    sizes.append(height)
    s.text(p.get_x()+p.get_width()/2,height +5,'{:1.2f}%'.format(height/len(df)*100), ha="center",fontsize=14)

s = sb.countplot(x = 'Parch',data = df) #celci pour affichage de diagramme
sizes=[]
for p in s.patches:
    height = p.get_height()
    sizes.append(height)
    s.text(p.get_x()+p.get_width()/2,height +5,'{:1.2f}%'.format(height/len(df)*100), ha="center",fontsize=14)

s = sb.countplot(x = 'Embarked',data = df) #celci pour affichage de diagramme
sizes=[]
for p in s.patches:
    height = p.get_height()
    sizes.append(height)
    s.text(p.get_x()+p.get_width()/2,height +5,'{:1.2f}%'.format(height/len(df)*100), ha="center",fontsize=14)

msno.bar(df)

msno.heatmap(df)

df['Cabin'].fillna(value=0,inplace=True)
df.loc[(df.Cabin !=0),'Cabin']=1

s = sb.countplot(x = 'Cabin',data = df) #celci pour affichage de diagramme
sizes=[]
for p in s.patches:
    height = p.get_height()
    sizes.append(height)
    s.text(p.get_x()+p.get_width()/2,height +5,'{:1.2f}%'.format(height/len(df)*100), ha="center",fontsize=14)

def names(dataset, name, t):
    for i in range(len(dataset['Name'])):
        if name in dataset['Name'][i]:
            dataset['Name'][i] = t

names(df, name='Miss.', t="miss")
names(df, name='Mrs.', t="mrs")
names(df, name='Mr.', t="mr")
names(df, name='Dr.', t="dr")
names(df, name='Master.', t="master")

mask_1 = df['Name'] != 'miss'
mask_2 = df['Name'] != 'mrs'
mask_3 = df['Name'] != 'mr'
mask_4 = df['Name'] != 'dr'
mask_5 = df['Name'] != 'master'

df.loc[mask_1 & mask_2 & mask_3 & mask_4 & mask_5, 'Name']='rien'

df_age1["Age"]=df_age1["Age"].interpolate(method='linear',limit_direction='forward',axis=0)

df_agemoy['Age'] = df_agemoy['Age'].replace(np.NaN,df_agemoy['Age'].mean()) #replacer les valeurs manquants par la moyenne
print(df_agemoy['Age'][:24])