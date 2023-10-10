package org.migcoincore.qt;

import android.os.Bundle;
import android.system.ErrnoException;
import android.system.Os;

import org.qtproject.qt5.android.bindings.QtActivity;

import java.io.File;

public class MigcoinQtActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        final File migcoinDir = new File(getFilesDir().getAbsolutePath() + "/.migcoin");
        if (!migcoinDir.exists()) {
            migcoinDir.mkdir();
        }

        super.onCreate(savedInstanceState);
    }
}
