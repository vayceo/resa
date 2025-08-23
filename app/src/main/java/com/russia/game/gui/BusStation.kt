package com.russia.game.gui

import com.russia.game.R
import com.russia.game.core.Samp.Companion.activity
import com.russia.game.databinding.BusStopBinding

class BusStation : NativeGui<BusStopBinding>(BusStopBinding::class) {

    fun update(time: Int) {
        activity.runOnUiThread {
            binding.timeText.text = String.format("%d", time)
        }
    }

    override fun receivePacket(actionId: Int, data: String) {
        TODO("Not yet implemented")
    }
}