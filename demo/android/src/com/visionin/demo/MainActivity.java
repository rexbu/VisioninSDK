package com.visionin.demo;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.visionin.Visionin;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Visionin.initialize(this, "f1a87ce5ae57f1e2484283419c3ef277", "e1f638ec0d52ac96b5e3fc8242813df7");
        Button button = (Button)findViewById(R.id.textureButton);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this, TextureActivity.class);
                MainActivity.this.startActivity(intent);
            }
        });

        Button facerButton = (Button)findViewById(R.id.bytesButton);
        facerButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this, BytesActivity.class);
                MainActivity.this.startActivity(intent);
            }
        });
    }
}
