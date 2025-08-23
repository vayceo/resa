package com.russia.game.gui.inventory

import android.content.res.ColorStateList
import android.graphics.Bitmap
import android.graphics.Color
import com.russia.game.SnapShot

class InventoryItem(
    var name: String = "",
    var count: String = "",
    var sprite: String = "",
    var rareColor: ColorStateList = ColorStateList.valueOf(Color.parseColor("#00000000")),
    var caption: String = ""
){

}