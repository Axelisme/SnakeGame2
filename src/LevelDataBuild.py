# %%
import numpy as np
obj = {
    "Air":0, 
    "Ground":1, 
    "Snake Head":2, 
    "Snake Body":3, 
    "End point":4,
    "Apple":5, 
    "Stone":6,
    "Spike":7,
    "Spike down":9,
    "Button":8,
    "Edge":-1
    }
def map_to_graph(map):
    map_graph = [''.join(
    '██' if j == obj["Ground"] else 
    '  ' if j==obj["Air"] else 
    '░░' if j==obj["Stone"] else 
    '▓▓' if j==obj["Snake Head"] else 
    '▒▒' if j==obj["Snake Body"] else  
    ' ⋄' if j==obj["Apple"] else 
    ' ⊠' if j==obj["End point"] else
    '||' if j==obj["Spike"] or j==obj["Spike down"] else
    ' ▂' if j==obj["Button"] else
    '▞▞' if j==obj["Edge"] else
    '  ' for j in i) + '\n' for i in map]
    return map_graph
def map_to_string(map):
    map_str = ['\t'.join(
    f'{obj["Ground"]}' if j == obj["Ground"] else 
    f'{obj["Air"]}' if j==obj["Air"] else 
    f'{obj["Stone"]}' if j==obj["Stone"] else 
    f'{obj["Snake Head"]}' if j==obj["Snake Head"] else 
    f'{obj["Snake Body"]}' if j==obj["Snake Body"] else  
    f'{obj["Apple"]}' if j==obj["Apple"] else
    f'{obj["End point"]}' if j==obj["End point"] else 
    f'{obj["Spike"]}' if j==obj["Spike"] else 
    f'{obj["Button"]}' if j==obj["Button"] else 
    f'{obj["Edge"]}' if j==obj["Edge"] else
    '0' for j in i) + '\n' for i in map]
    return map_str

def add_edge(map, thick = 7):
    high, width = map.shape
    edge_col = np.full((high,thick),obj["Air"])
    edge_row = np.full((thick,width+2*thick),obj["Air"])
    map = np.concatenate([edge_col, map], axis=1)
    map = np.concatenate([map, edge_col], axis=1)
    map = np.concatenate([edge_row, map], axis=0)
    map = np.concatenate([map, edge_row], axis=0)
    map[0:2,:] = map[-2:,:] = map[:,0:2] = map[:,-2:] = obj["Edge"]
    return map

def write_file(id,level):
    with open(f'../data/level/L{id}','w') as fh:
        fh.write('\n'.join(
            [
                "1background", str(len(level["background"])),*level["background"],'',
                "2music", str(len(level["music"])),*level["music"],'',
                "3snake", str(len(level["snake"])), *level["snake"],'',
                "4stone", str(len(level["stone"])), *level["stone"],'',
                "5button", str(len(level["button"])), *level["button"],'',
                "6map", f'{level["map"][0][0]} {level["map"][0][1]}',''
            ]
        )+'\n')
        fh.writelines([
            *level["map"][1], *level["map"][2]
        ])
    return

map1_high, map1_width = 10, 25
map1 = np.zeros([map1_high, map1_width]).astype(int)
map1[1:4,17] = obj["Ground"]
map1[6,10:24] = obj["Ground"]
map1[6,14:17] = obj["Air"]
map1[4,4] = obj["Ground"]
map1[9,24] = obj["Ground"]
map1[5,1:13] = obj["Ground"]
map1[5,6:8] = obj["Air"]
map1[5,11] = obj["Air"]
map1[2,9:14] = obj["Ground"]
map1[2,11] = obj["Air"]
map1[3,[9,12]] = obj["Ground"]
map1[4,12] = obj["Ground"]
map1[0,17] = obj["Stone"]
map1[3,10] = obj["Apple"]
map1[3,18] = obj["Apple"]
map1[7,19] = obj["End point"]
map1[4,1] = obj["Snake Head"]
map1[4,0] = obj["Snake Body"]
map1[5,0] = obj["Snake Body"]
map1 = add_edge(map1)
map1_list = map1.tolist()
map1_graph = map_to_graph(map1)
map1_str = map_to_string(map1)

level1 = {
    "background": ['/data/image/background1'],
    "music": ['/data/music/music1'],
    "sound": ['/data/sound/sound1-1','/data/sound/sound1-2'],
    "map":[map1.shape, map1_str, map1_graph],
    "snake":[
        '12 7',
        '11 7',
        '11 8',
    ],
    "stone":[
        '7 24'
        ],
    "button":[
    ]
    }

write_file(1,level1)

map2_high, map2_width = 10, 25
map2 = np.zeros([map2_high, map2_width]).astype(int)
map2[3:8,1] = obj["Ground"]
map2[[3,7],2] = obj["Ground"]
map2[1:5,4] = obj["Ground"]
map2[2:4,5] = obj["Ground"]
map2[1:5,4] = obj["Ground"]
map2[6,4:18] = obj["Ground"]
map2[6,[8,16]] = obj["Air"]
map2[3:9,7] = obj["Ground"]
map2[7,9] = obj["Ground"]
map2[8,8:10] = obj["Ground"]
map2[3:5,9] = obj["Ground"]
map2[4,10] = obj["Ground"]
map2[2,13:21] = obj["Ground"]
map2[2,14] = obj["Air"]
map2[3,[15,18]] = obj["Ground"]
map2[3:5,19:21] = obj["Ground"]
map2[7,15:19] = obj["Ground"]
map2[7,21:25] = obj["Ground"]
map2[8,18:23] = obj["Ground"]
map2[6,21:23] = obj["Ground"]
map2[5,17] = obj["Ground"]
map2[4:6,13:15] = obj["Ground"]
map2[[2,5,4,5,5],[9,12,14,19,22]] = obj["Spike"]
map2[[6,5,2],[3,16,12]] = obj["Apple"]
map2[[1,5],[5,15]] = obj["Button"]
map2[6,24] = obj["End point"]
map2[2:4,0] = obj["Snake Body"]
map2[2,1] = obj["Snake Head"]
map2 = add_edge(map2)
map2_list = map2.tolist()
map2_graph = map_to_graph(map2)
map2_str = map_to_string(map2)

level2 = {
    "background": ['/data/image/background2'],
    "music": ['/data/music/music2'],
    "sound": ['/data/sound/sound2-1','/data/sound/sound2-2'],
    "map":[map2.shape, map2_str, map2_graph],
    "snake":[
        '10 7',
        '9 7',
        '9 8',
    ],
    "stone":[
        ],
    "button":[
        '8 12 Short',
        '''12 22 Spike
3
11 21
8 22
8 20''',
    ]
    }

write_file(2,level2)


map3_high, map3_width = 10, 25
map3 = np.zeros([map3_high, map3_width]).astype(int)
map3[1,3] = obj["Apple"]
map3[2:4,3] = obj["Ground"]
map3[6,2] = obj["Ground"]
map3[5,3] = obj["Snake Head"]
map3[4:6,1:3] = obj["Snake Body"]
map3[4,2] = obj["Air"]
map3[5,2] = obj["End point"]

map3 = add_edge(map3)
map3_list = map3.tolist()
map3_graph = map_to_graph(map3)
map3_str = map_to_string(map3)

level3 = {
    "background": ['/data/image/background3'],
    "music": ['/data/music/music3'],
    "sound": ['/data/sound/sound3-1','/data/sound/sound3-2'],
    "map":[map3.shape, map3_str, map3_graph],
    "snake":[
        '11 8',
        '12 8',
        '12 9',
        '12 10',
        ],
    "stone":[
        ],
    "button":[
        ]
    }

write_file(3,level3)

map4_high, map4_width = 10, 25
map4 = np.zeros([map4_high, map4_width]).astype(int)
map4[1,17:19] = map4[9,17:23] = map4[9,17:23] = map4[7,10:15] = map4[5,15:17] = map4[5,6:8] = map4[2,0:2] = map4[3,1:3] = map4[9,1:4] = map4[8,1:3] = obj["Ground"]
map4[6:10,1] = map4[3:6,12] = obj["Ground"]
map4[[7,7,9,8,2,5,8],[4,6,7,11,20,24,24]] = obj["Ground"]
map4[[6,6,2,2],[4,10,12,17]] = obj["Spike"]
map4[[6,8],[12,22]] = obj["Button"]
map4[3:5,6:8] = obj["Snake Body"]
map4[3,6] = obj["Snake Head"]
map4[4,0] = obj["Apple"]
map4[4,15] = obj["Stone"]
map4[4,24] = obj["End point"]

map4 = add_edge(map4)
map4_list = map4.tolist()
map4_graph = map_to_graph(map4)
map4_str = map_to_string(map4)

level4 = {
    "background": ['/data/image/background3'],
    "music": ['/data/music/music3'],
    "sound": ['/data/sound/sound3-1','/data/sound/sound3-2'],
    "map":[map4.shape, map4_str, map4_graph],
    "snake":[
        '10 14',
        '11 14',
        '11 13',
        '10 13',
        ],
    "stone":[
        '11 22'
        ],
    "button":[
        '13 19 Short',
        '15 29 Grave'
        ]
    }

write_file(4,level4)