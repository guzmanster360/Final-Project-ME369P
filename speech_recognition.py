# M E 369P Final Project
import speech_recognition as sr
import pyaudio
import time
def filter_words(recognized_words):
    
def real_time_speech_to_text():
    recognizer = sr.Recognizer()
    all_text = ""  # Initialize an empty string to store all transcriptions

    # Define the list of directional words to filter for
    directional_words = {"clockwise", "counterclockwise", "degrees", "hover", "then", }

    # Use the default microphone as the audio source
    with sr.Microphone() as source:
        print("Adjusting for ambient noise... please wait.")
        recognizer.adjust_for_ambient_noise(source)
        print("Listening... Start speaking.")

        # Continuously listen and recognize speech
        while True:
            try:
                # Capture the audio
                print("Countdown: Listening in ")
                time.sleep(1)
                print("3")
                time.sleep(1)
                print("2")
                time.sleep(1)
                print("1")
                time.sleep(1)
                print("GO!")
                
                audio_data = recognizer.listen(source, timeout = 10)
                
                # Transcribe the audio using Google’s Web Speech API
                text = recognizer.recognize_google(audio_data)
                corrected_text = text.replace("°", " degrees")
                print("You said: " + corrected_text)

                # Filter only directional words
                filtered_words = [word for word in corrected_text.split() if word.lower() in directional_words or word.isdigit()]
                filtered_text = " ".join(filtered_words)

                # Append the filtered text to all_text
                all_text += filtered_text + " "

            except sr.UnknownValueError:
                print("Sorry, I did not understand that.")
            except sr.RequestError:
                print("Could not request results from Google Speech Recognition service.")
            except KeyboardInterrupt:
                print("\nExiting real-time transcription.")
                break

    return all_text  # Return the accumulated transcription of directional words only

# Run the real-time speech-to-text function and save the result
transcribed_directional_text = real_time_speech_to_text()
print("Directional Transcription:\n", transcribed_directional_text)
