package com.russia.launcher.ui.adapters;

import android.content.Intent;

import android.os.Handler;
import android.view.LayoutInflater;
import android.view.animation.AnimationUtils;
import android.view.animation.Animation;
import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.recyclerview.widget.RecyclerView;
import androidx.annotation.NonNull;

import com.russia.game.R;
import com.russia.launcher.ui.activity.StoryActivity;
import com.russia.launcher.async.dto.response.News;

import java.util.List;

import com.bumptech.glide.Glide;

public class NewsAdapter extends RecyclerView.Adapter<NewsAdapter.NewsViewHolder> {
	Context context;
	List<News> news;
	
	public NewsAdapter(Context context, List<News> news){
		 this.context = context;
		 this.news = news;
	}
	
	@NonNull
	@Override
    public NewsViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View v = LayoutInflater.from(context).inflate(R.layout.item_news, parent, false);
		return new NewsViewHolder(v); 
    }
  
    @Override
    public void onBindViewHolder(@NonNull NewsViewHolder holder, int position) {
        News news = this.news.get(position);
		holder.title.setText(news.getTitle());
		Glide.with(context).load(news.getImageUrl()).into(holder.image);
		holder.container.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                view.startAnimation(AnimationUtils.loadAnimation(context ,R.anim.button_click));
                new Handler().postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        startStorySlider(position);
                    }
                }, 200);
            }
        });
    }

    public void startStorySlider(int pos) {
        Intent intent = new Intent(context, StoryActivity.class);
        intent.putExtra("position", pos);
        context.startActivity(intent);
    }

    @Override
    public int getItemCount() {
        return news.size();
    }

    public static class NewsViewHolder extends RecyclerView.ViewHolder {
        
		TextView title;
		ImageView image;
		FrameLayout container;

        public NewsViewHolder(View itemView) {
            super(itemView);
            
		    title = itemView.findViewById(R.id.title);
			image = itemView.findViewById(R.id.image);
			container = itemView.findViewById(R.id.container);
        }
    }
	
	private void setAnimation(View view) {	
    Animation anim = AnimationUtils.loadAnimation(context, R.anim.button_click);
    view.startAnimation(anim);
}
}