package tech.smallwonder.spaceshooterdeluxe;

import org.liballeg.android.AllegroActivity;

public class MainActivity extends AllegroActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("allegro");
        System.loadLibrary("allegro_font");
        System.loadLibrary("allegro_ttf");
        System.loadLibrary("allegro_audio");
        System.loadLibrary("allegro_acodec");
        System.loadLibrary("allegro_primitives");
        System.loadLibrary("allegro_image");
    }

    public MainActivity() {
        super("libnative-lib.so");
    }


}
