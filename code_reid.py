from music21 import converter, note, chord
import time

# Load the MusicXML file
xml_file = "Beethoven7th_chorus_only.mxl"  # Replace with your file path
score = converter.parse(xml_file)

note_data = []  # Stores (absolute_time, [notes], duration, part_name)

for part in score.parts:  
    part_name = part.partName if part.partName else "Unknown Part"
    
    for element in part.flat.notesAndRests:  # This includes rests!
        absolute_time = element.offset  # Time from start of piece

        if isinstance(element, note.Note):
            note_name = element.nameWithOctave
            duration = element.quarterLength
            note_data.append((absolute_time, [note_name], duration, part_name))

        elif isinstance(element, chord.Chord):
            chord_notes = [n.nameWithOctave for n in element.notes]
            duration = element.quarterLength
            note_data.append((absolute_time, chord_notes, duration, part_name))

        elif isinstance(element, note.Rest):  # Detecting rests
            duration = element.quarterLength
            note_data.append((absolute_time, ["Rest"], duration, part_name))

# Sort data by absolute time
note_data.sort(key=lambda x: x[0])

# Print formatted output
for entry in note_data:
    absolute_time, notes, duration, part_name = entry
    print(f"Time: {absolute_time:.2f} beats | Notes: {notes} | Duration: {duration} beats | Part: {part_name}")


def convertNOTE(note):

        if note[0:1] == "C":
            num = 12
        if note[0:2] == "C#":
            num = 11
        if note[0:1] == "D":
            num = 10
        if note[0:2] == "D#":
            num = 9
        if note[0:1] == "E":
            num = 8
        if note[0:1] == "F":
            num = 7
        if note[0:2] == "F#":
            num = 6
        if note[0:1] == "G":
            num = 5
        if note[0:2] == "G#":
            num = 4
        if note[0:1] == "A":
            num = 3
        if note[0:2] == "A#":
            num = 2
        if note[0:1] == "B":
            num = 1

        if note[-1] == "t":
            return note

        return int(note[-1]) * 12 - num

time.sleep(5)

for entry in note_data:
    absolute_time, notes, duration, part_name = entry

    if part_name[0:1] == "A":

        if absolute_time == duration:
            time.sleep(duration)

        for note in notes:
            print(str(convertNOTE(note)) + " on")

        time.sleep(duration)
            
        for note in notes:
            print(str(convertNOTE(note)) + " off")
    