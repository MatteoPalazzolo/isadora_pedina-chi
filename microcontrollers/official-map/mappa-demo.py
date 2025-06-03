from dataclasses import dataclass

@dataclass
class Sensor:
    name: str
    prvsState: int = 0
    score: int = 0
    zero_counter: int = 0

max_score: int = 0
sensors: list[Sensor] = [
    Sensor(prvsState=0, score=0, name="S1"),
    Sensor(prvsState=0, score=0, name="S2"),
    Sensor(prvsState=0, score=0, name="S3")
]

pedine: list[str] = [
    "None",
    "Fox",
    "Pawn",
    "Rose"
]

def main(code: str):
    global max_score
    for pwnId, sensor in zip(map(int,code), sensors):
        # se il sensore ha cambiato valore
        if sensor.prvsState != pwnId:
            if pwnId == 0:
                # conta fino a 5 prima di comunicare la rimozione di una pedina
                sensor.zero_counter += 1
                if sensor.zero_counter >= 5:
                    sensor.score = 0
                    sensor.prvsState = 0
                    sensor.zero_counter = 0
            else:
                max_score += 1
                sensor.score = max_score
                sensor.prvsState = pwnId
                sensor.zero_counter = 0


while True:
    main(input("una sequenza di 3 cifre (0, 1, 2 o 3): "))
    print("\n".join(map(str,sensors)))
    print("il sensore attivo è:", max(sensors,key=lambda s: s.score).name)
    print("la pedina attiva è:", pedine[max(sensors,key=lambda s: s.score).prvsState])