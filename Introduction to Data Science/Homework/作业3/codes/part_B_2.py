import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

file_path = "dataset/ai_job_trends_dataset_cleaned.csv"
df = pd.read_csv(file_path)

# 新增绝对变化列
df['Openings Abs Change'] = df['Projected Openings (2030)'] - df['Job Openings (2024)']

# 按 Industry 和 Location 分组，计算总岗位变化
industry_change = df.groupby('Industry')['Openings Abs Change'].sum().sort_values(ascending=True)
location_change = df.groupby('Location')['Openings Abs Change'].sum().sort_values(ascending=True)

# 可视化
plt.figure(figsize=(14, 10))
# 行业变化
plt.subplot(2, 1, 1)
sns.barplot(
    x=industry_change.values,
    y=industry_change.index,
    hue=industry_change.index,
    palette='viridis',
    legend=False
)
plt.title('Net Job Opening Change (2024→2030) by Industry', fontsize=14)
plt.xlabel('Total Change in Job Openings')
plt.ylabel('Industry')
# 地区变化
plt.subplot(2, 1, 2)
sns.barplot(
    x=location_change.values,
    y=location_change.index,
    hue=location_change.index,
    palette='magma',
    legend=False
)
plt.title('Net Job Opening Change (2024→2030) by Location', fontsize=14)
plt.xlabel('Total Change in Job Openings')
plt.ylabel('Location')
plt.tight_layout()
plt.savefig("imgs/job_opening_changes.png", dpi=300, bbox_inches='tight')
plt.show()

# 热力图可视化
cross_agg = df.groupby(['Industry', 'Location'])['Openings Abs Change'].sum().reset_index()
heatmap_data = cross_agg.pivot_table(
    index='Industry',
    columns='Location',
    values='Openings Abs Change',
    fill_value=0 
)
industry_order = heatmap_data.sum(axis=1).sort_values().index
heatmap_data = heatmap_data.reindex(industry_order)
location_order = heatmap_data.sum(axis=0).sort_values().index
heatmap_data = heatmap_data[location_order]
plt.figure(figsize=(16, 10))
sns.heatmap(
    heatmap_data,
    annot=True,
    fmt=".0f",
    cmap="RdYlGn",  
    center=0,      
    cbar_kws={'label': 'Net Job Opening Change (2024→2030)'},
    linewidths=0.5,
    square=False
)
plt.title('Net Job Opening Change by Industry and Location (2024–2030)', fontsize=16, pad=20)
plt.xlabel('Location')
plt.ylabel('Industry')
plt.yticks(rotation=0)
plt.tight_layout()
plt.savefig('imgs/net_job_openings_heatmap.png')
plt.show()

# 热点
top_growth = cross_agg.nlargest(5, 'Openings Abs Change')
print("\nTop 5 (Industry, Location) with Strongest Job Growth:")
print(top_growth.to_string(index=False))
# 冷点
top_decline = cross_agg.nsmallest(5, 'Openings Abs Change')
print("\nTop 5 (Industry, Location) with Severe Job Decline:")
print(top_decline.to_string(index=False))