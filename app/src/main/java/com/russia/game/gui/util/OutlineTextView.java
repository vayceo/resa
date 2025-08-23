package com.russia.game.gui.util;

import static java.security.AccessController.getContext;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.widget.TextView;

import androidx.annotation.NonNull;

import com.russia.game.R;

public class OutlineTextView extends androidx.appcompat.widget.AppCompatTextView {
    public OutlineTextView(@NonNull Context context) {
        super(context);
    }

    // Constructors

    @Override
    public void draw(Canvas canvas) {
        for (int i = 0; i < 5; i++) {
            super.draw(canvas);
        }
    }

}

