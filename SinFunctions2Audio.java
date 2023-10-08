import javax.sound.sampled.AudioFileFormat;
import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.util.Collection;

public class SinFunctions2Audio {
    public void createLinearCombo (Collection<SinData> sinData) {
        // create linear combination of sin waves
        int SAMPLE_RATE = 44100;
        double DURATION = 5.0;
        int NUM_CHANNELS = 1;

        AudioFormat audioFormat = new AudioFormat(SAMPLE_RATE, 16, NUM_CHANNELS, true, true);

        try {
            ByteArrayOutputStream audioData = new ByteArrayOutputStream();
            long numFrames = (long) (DURATION * SAMPLE_RATE);
            int numBytes = (int) (numFrames * audioFormat.getFrameSize());

            for(long frame = 0; frame < numFrames; frame++) {
                double sample = 0.0;
                double time = frame / (double) SAMPLE_RATE;

                for (SinData sd : sinData) {
                    sample += sd.getAmplitude() * Math.sin(2 * Math.PI * sd.getFrequency() * time);
                }

                // sample is in range [-1.0, 1.0], so this converts to range [-32767, 32767] for 16-bit audio
                audioData.write((int) (sample * Short.MAX_VALUE));
            }

            byte[] audioBytes = audioData.toByteArray();

            AudioInputStream audioInputStream = new AudioInputStream(
                    new ByteArrayInputStream(audioBytes),
                    audioFormat,
                    audioBytes.length / audioFormat.getFrameSize()
            );

            File audioFile = new File("audio.wav");
            AudioSystem.write(audioInputStream, AudioFileFormat.Type.WAVE, audioFile);
        } catch (IOException io) {
            System.out.println("Error writing to file");
        }
    }
}
