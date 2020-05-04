package gisia.martin.com.perceptron;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.Camera;
import android.net.Uri;
import android.provider.MediaStore;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.ByteArrayOutputStream;
import java.nio.ByteBuffer;

public class MainActivity extends AppCompatActivity {
    TextView tvResultado;
    ImageView imageView;
    Bitmap bitmapSelected;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        imageView = findViewById(R.id.imageView);
        tvResultado = (TextView) findViewById(R.id.tvResultado);
        //convolucionModel();
        //pool();
        redConvolucion();
        //float result = modelTraining();
        float result = 2;
        tvResultado.setText("El resultado es: "+ String.valueOf(result));
        Toast.makeText(this, "Resultado "+ result, Toast.LENGTH_LONG).show();





        int permissionCheck = ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE);

        if (permissionCheck != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 12);
            openGallerySelectImage();
        } else {
            openGallerySelectImage();
        }
    }

    private void openGallerySelectImage() {
        Intent intent = new Intent();
        intent.setType("image/*");
        intent.setAction(Intent.ACTION_GET_CONTENT);
        startActivityForResult(Intent.createChooser(intent, "Select Picture"), PICK_IMAGE);
    }

    public static final int PICK_IMAGE = 1;

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        if (requestCode == PICK_IMAGE) {
            setImageView(data);
            //redConvolucion(bitmapSelected);
        }
    }

    private void setImageView(Intent data) {
        Uri selectedImage = data.getData();
        String[] filePathColumn = { MediaStore.Images.Media.DATA };

        Cursor cursor = getContentResolver().query(selectedImage,
                filePathColumn, null, null, null);
        cursor.moveToFirst();

        int columnIndex = cursor.getColumnIndex(filePathColumn[0]);
        String picturePath = cursor.getString(columnIndex);
        cursor.close();

        bitmapSelected = BitmapFactory.decodeFile(picturePath);
        imageView.setImageBitmap(bitmapSelected);

        int size = bitmapSelected.getRowBytes() * bitmapSelected.getHeight();
        ByteBuffer byteBuffer = ByteBuffer.allocate(size);
        bitmapSelected.copyPixelsToBuffer(byteBuffer);
        byte[] byteArray = byteBuffer.array();
        //redConvolucion();
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native float modelTraining();

    public native void convolucionModel();

    public native void pool();

    public native void redConvolucion();

}
