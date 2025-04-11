from music21 import converter, note, chord

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
