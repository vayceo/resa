package com.russia.game.gui.casino

import android.content.res.ColorStateList
import com.russia.game.R
import com.russia.game.core.Samp.Companion.activity
import com.russia.game.databinding.CasinoLuckyWheelBinding
import com.russia.game.gui.NativeGui
import java.text.SimpleDateFormat
import java.util.Date

class LuckyWheel : NativeGui<CasinoLuckyWheelBinding>(CasinoLuckyWheelBinding::class) {
    private external fun nativeSendClick(buttonID: Int)

    init {
        activity.runOnUiThread {
            binding.casinoLuckywheelSpeenFreeButt.setOnClickListener {
                nativeSendClick(0)
            }
            binding.casinoLuckywheelSpeenPayButt.setOnClickListener {
                nativeSendClick(1)
            }
            binding.casinoLuckywheelExitButt.setOnClickListener {
                nativeSendClick(228)
            }

        }
    }

    private fun show(count: Int, time: Int) {
        activity.runOnUiThread {
            val color: ColorStateList = if (count <= 0) {
                ColorStateList.valueOf(activity.resources.getColor(R.color.gray))
            } else {
                ColorStateList.valueOf(activity.resources.getColor(R.color.blue_))
            }

            binding.casinoLuckywheelSpeenFreeButt.backgroundTintList = color
            binding.casinoLuckywheelCount.text = String.format("%d", count)
            if (count <= 0) {
                val timeleft = SimpleDateFormat("HH:mm").format(Date((time * 1000).toLong()))
                binding.casinoLuckywheelSpeenFreeButt.text = "Доступно через $timeleft"
            } else {
                binding.casinoLuckywheelSpeenFreeButt.text = "бесплатная прокрутка"
            }

        }
    }

    override fun receivePacket(actionId: Int, data: String) {
        TODO("Not yet implemented")
    }
}