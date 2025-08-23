package com.russia.game.gui

import android.os.Handler
import android.view.View
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.russia.game.R
import com.russia.game.core.Samp
import com.russia.game.core.Samp.Companion.activity
import com.russia.game.databinding.MenuActionDialogBinding
import com.russia.game.gui.adapters.DialogMenuAdapter
import com.russia.game.gui.adapters.DialogMenuAdapter.OnUserClickListener
import com.russia.game.gui.models.DataDialogMenu
import com.rommansabbir.animationx.Slide
import com.rommansabbir.animationx.animationXSlide

class Menu : NativeGui<MenuActionDialogBinding>(MenuActionDialogBinding::class) {

    private var index = -1
    private val dataDialogMenuArrayList = ArrayList<DataDialogMenu>()

    private external fun nativeSendMenuButt(buttId: Int)

    init {
        activity.runOnUiThread {
            binding.closeButton.setOnClickListener {
                close()
            }
            update(false)
            binding.mainLayout.animationXSlide(Slide.SLIDE_IN_UP, duration = 300)
        }
    }

    fun update(z: Boolean) {

        val recyclerView = activity.findViewById<RecyclerView>(R.id.br_rec_view_menu)
        index = -1

        if (!z) {
            setMenu()
            binding.caption.text = "Действия"
            setDataInRecyclerView({ dataDialogMenu: DataDialogMenu, view: View? ->
                view?.startAnimation(Samp.clickAnim)
                index = dataDialogMenu.id
                Handler().postDelayed({
                    close()
                    nativeSendMenuButt(index)
                }, 200)
            }, dataDialogMenuArrayList, recyclerView, binding.root, 4)
        }
    }

    private fun setMenu() {
        dataDialogMenuArrayList.clear()
        dataDialogMenuArrayList.add(DataDialogMenu(1, R.drawable.menu_icon_gps, "Навигатор"))
        dataDialogMenuArrayList.add(DataDialogMenu(2, R.drawable.menu_icon_mm, "Меню"))
        // this.dataDialogMenuArrayList.add(new DataDialogMenu(3, R.drawable.br_menu_chat, "Общение"));
        dataDialogMenuArrayList.add(DataDialogMenu(3, R.drawable.menu_icon_inv, "Инвентарь"))
        dataDialogMenuArrayList.add(DataDialogMenu(4, R.drawable.menu_icon_anim, "Анимации"))
        dataDialogMenuArrayList.add(DataDialogMenu(5, R.drawable.br_menu_ruble, "Донат"))
        dataDialogMenuArrayList.add(DataDialogMenu(6, R.drawable.menu_icon_car, "Автомобили"))
        dataDialogMenuArrayList.add(DataDialogMenu(7, R.drawable.menu_report_icon, "Жалоба"))
        dataDialogMenuArrayList.add(DataDialogMenu(8, R.drawable.menu_promocode_icon, "Промокод"))
        dataDialogMenuArrayList.add(DataDialogMenu(9, R.drawable.players_icon, "Игроки"))
        dataDialogMenuArrayList.add(DataDialogMenu(10, R.drawable.menu_icon_family, "Семья"))
        dataDialogMenuArrayList.add(DataDialogMenu(11, R.drawable.menu_icon_achivments, "Достижения"))
        dataDialogMenuArrayList.add(DataDialogMenu(12, R.drawable.menu_icon_battlepass, "LIVE PASS"))
    }

    private fun setDataInRecyclerView(onUserClickListener: OnUserClickListener, arrayList: ArrayList<DataDialogMenu>, recyclerView: RecyclerView, view: View, i: Int) {
        val dialogMenuAdapter = DialogMenuAdapter(arrayList, onUserClickListener)
        recyclerView.layoutManager = object : GridLayoutManager(view.context, i) {
            override fun checkLayoutParams(layoutParams: RecyclerView.LayoutParams): Boolean {
                val f = 30.0f / view.resources.displayMetrics.density
                val i2 = f.toInt()
                layoutParams.marginStart = i2
                layoutParams.marginEnd = i2
                layoutParams.setMargins(0, i2, 0, 0)
                layoutParams.width = ((width / spanCount).toFloat() - f).toInt()
                return true
            }
        }
        recyclerView.adapter = dialogMenuAdapter

    }

    fun close() {
        binding.mainLayout.animate().translationYBy(2000.0f)
            .alpha(0f)
            .setDuration(300)
            .withEndAction {
                binding.mainLayout.visibility = View.GONE
                binding.mainLayout.translationX = 0f
                binding.mainLayout.alpha = 1f

                destroy()
            }
            .start()
    }

    override fun receivePacket(actionId: Int, data: String) {
        TODO("Not yet implemented")
    }
}