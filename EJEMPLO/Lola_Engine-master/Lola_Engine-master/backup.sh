# First, we delete all the files on the Drive folder...
echo "Deleting files on Engine Drive's folder"
rm -fr  ~/Google\ Drive/Grado\ Videojuego\ -\ CITM/Curso3_Semestre1/Motores/*

# Next, we copy engine files to this Google Drive folder
echo "Copying files to Google Drive folder..."
cp -urf ./* ~/Google\ Drive/Grado\ Videojuego\ -\ CITM/Curso3_Semestre1/Motores/ 
echo "Files copied! Starting Google Drive..."


# Starting Drive to initiate syncronization!
"C:\Program Files (x86)\Google\Drive\googledrivesync.exe" &

# Completed!
echo "See you Carlos! May the force be with you..."
