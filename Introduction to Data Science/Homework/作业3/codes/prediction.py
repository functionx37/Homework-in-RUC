import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestRegressor
from sklearn.metrics import r2_score, mean_squared_error
import numpy as np
import matplotlib.pyplot as plt

# 读取数据集
file_path = "dataset/ai_job_trends_dataset_cleaned.csv"
df = pd.read_csv(file_path)
required_columns = [
    'Industry',
    'Required Education',
    'Experience Required (Years)',
    'Location',
    'AI Impact Level',
    'Automation Risk (%)',
    'Job Status',
    'Median Salary (USD)'
]
df = df[required_columns]

# 数据编码
education_map = {
    'High School': 1,
    'Associate Degree': 2,
    'Bachelor’s Degree': 3,
    'Master’s Degree': 4,
    'PhD': 5
}
df['Required Education'] = df['Required Education'].map(education_map)
job_status_map = {
    'Decreasing': -1,
    'Increasing': 1
}
df['Job Status'] = df['Job Status'].map(job_status_map)
ai_impact_map = {
    'Low': 1,
    'Moderate': 2,
    'High': 3
}
df['AI Impact Level'] = df['AI Impact Level'].map(ai_impact_map)
df = pd.get_dummies(df, columns=['Industry', 'Location'], drop_first=True)

# 分离特征和目标变量
X = df.drop(columns=['Median Salary (USD)'])
y = df['Median Salary (USD)']

# 划分训练集和测试集
X_train, X_test, y_train, y_test = train_test_split(
    X, y, 
    train_size=24000, test_size=6000, 
    random_state=42
)

# 训练随机森林回归器
rf = RandomForestRegressor(random_state=42)
rf.fit(X_train, y_train)

# 预测与评估
y_pred = rf.predict(X_test)
r2 = r2_score(y_test, y_pred)
rmse = np.sqrt(mean_squared_error(y_test, y_pred))
mean_y = y_test.mean()
rmse_ratio = rmse / mean_y
print(f"R² Score: {r2:.4f}")
print(f"RMSE: {rmse:.2f}")
print(f"RMSE / Mean of y (Relative RMSE): {rmse_ratio:.4f} ({rmse_ratio*100:.2f}%)")

# 模型可视化  

# 散点图
plt.figure(figsize=(8, 6))
plt.scatter(y_test, y_pred, alpha=0.6)
plt.plot([y_test.min(), y_test.max()], [y_test.min(), y_test.max()], 'r--', lw=2)
plt.xlabel("True Median Salary (USD)")
plt.ylabel("Predicted Median Salary (USD)")
plt.title("True vs Predicted Salary")
plt.grid(True)
plt.tight_layout()
plt.savefig("imgs/true_vs_predicted.png", dpi=300, bbox_inches='tight')
plt.show()
# 残差图
residuals = y_test - y_pred
plt.figure(figsize=(8, 6))
plt.scatter(y_pred, residuals, alpha=0.6)
plt.axhline(0, color='red', linestyle='--')
plt.xlabel("Predicted Salary (USD)")
plt.ylabel("Residuals (True - Pred)")
plt.title("Residuals vs Predicted")
plt.grid(True)
plt.tight_layout()
plt.savefig("imgs/residuals_vs_predicted.png", dpi=300, bbox_inches='tight')
plt.show()
# 误差分布直方图
plt.figure(figsize=(8, 6))
plt.hist(residuals, bins=30, edgecolor='k', alpha=0.7)
plt.axvline(0, color='red', linestyle='--')
plt.xlabel("Prediction Error (Residual)")
plt.ylabel("Frequency")
plt.title("Distribution of Prediction Errors")
plt.grid(True)
plt.tight_layout()
plt.savefig("imgs/residuals_distribution.png", dpi=300, bbox_inches='tight')
plt.show()
# 特征重要性条形图
feature_importances = pd.Series(rf.feature_importances_, index=X_train.columns)
top_features = feature_importances.nlargest(10)
plt.figure(figsize=(10, 6))
top_features.plot(kind='barh')
plt.title("Top 10 Feature Importances")
plt.xlabel("Importance")
plt.gca().invert_yaxis()
plt.tight_layout()
plt.savefig("imgs/feature_importances.png", dpi=300, bbox_inches='tight')
plt.show()