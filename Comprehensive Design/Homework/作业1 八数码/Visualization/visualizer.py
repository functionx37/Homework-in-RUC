import pygame
import subprocess
import os
import time
import sys

# --- 常量定义 ---
# 颜色
WHITE, BLACK, GREEN, LIGHT_GREEN = (255, 255, 255), (0, 0, 0), (67, 160, 71), (129, 199, 132)
BACKGROUND, GRAY, BLUE_GRAY, RED = (236, 239, 241), (189, 189, 189), (96, 125, 139), (211, 47, 47)
OBSTACLE_COLOR = (45, 45, 45) # 障碍物颜色
# 动画速度
ANIMATION_DURATION = 0.25
# 游戏状态
class GameState:
    WAITING_FOR_START, ANIMATING, NO_SOLUTION, FINISHED = range(4)

# --- 文本输入函数 (修改验证逻辑) ---
def get_puzzles_from_console():
    """在终端读取用户输入，并动态推断谜题尺寸 N"""
    print("="*60)
    print("请输入 N*N 谜题数据 (可包含 -1 作为障碍物)")
    print("请直接粘贴初始和目标状态，然后在一个空行处按 Enter 键开始：")
    print("="*60)
    lines = []
    while True:
        try:
            line = input()
            if not line.strip():
                if lines: break
            else: lines.append(line)
        except EOFError: break
    try:
        if not lines or len(lines) % 2 != 0: raise ValueError(f"输入行数必须是偶数，但收到了 {len(lines)} 行。")
        puzzle_size = len(lines) // 2
        if puzzle_size < 2: raise ValueError("谜题尺寸至少为 2x2。")
        
        all_rows = [[int(n) for n in line.split()] for line in lines]
        if any(len(row) != puzzle_size for row in all_rows): raise ValueError(f"所有行都应包含 {puzzle_size} 个数字。")

        # ============== 关键修改 1: 验证逻辑 ==============
        # 验证初始和目标状态是否是有效的排列关系 (拥有相同的元素)
        start_list_flat = sorted([num for r in all_rows[:puzzle_size] for num in r])
        goal_list_flat = sorted([num for r in all_rows[puzzle_size:] for num in r])
        if start_list_flat != goal_list_flat:
            raise ValueError("初始状态和目标状态的元素集合不匹配。")
        if 0 not in start_list_flat:
             raise ValueError("谜题必须包含一个空格(0)。")

        start_board = tuple(map(tuple, all_rows[:puzzle_size]))
        goal_board = tuple(map(tuple, all_rows[puzzle_size:]))
        return start_board, goal_board, puzzle_size
    except (ValueError, IndexError) as e:
        print(f"\n[解析错误] {e}")
        return None, None, None

# --- C++ 调用函数 (不变) ---
def get_solution_path_from_cpp(start_state: tuple, goal_state: tuple) -> list[tuple] | None:
    solver_executable = "solver.exe" if os.name == 'nt' else "./solver"
    if not os.path.exists(solver_executable.replace("./", "")): 
        print(f"错误: 求解器 '{solver_executable}' 不存在。")
        return None
    flat_start = [str(item) for row in start_state for item in row]
    flat_goal = [str(item) for row in goal_state for item in row]
    command = [solver_executable] + flat_start + flat_goal
    try:
        result = subprocess.run(command, capture_output=True, text=True, check=True)
    except subprocess.CalledProcessError as e:
        print(f"C++ 求解器信息: {e.stderr.strip()}")
        return None
    output = result.stdout.strip()
    if not output: return None
    solution_path = []
    n = len(start_state)
    for line in output.split('\n'):
        nums = [int(num) for num in line.split() if num]
        if len(nums) == n * n:
            solution_path.append(tuple(tuple(nums[i*n : (i+1)*n]) for i in range(n)))
    return solution_path

# --- 动画绘制函数 (修改绘制逻辑) ---
def get_tile_positions(board: tuple, tile_size: int, margin: int) -> dict:
    positions = {}
    for r, row in enumerate(board):
        for c, val in enumerate(row):
            if val != 0: positions[val] = (c*(tile_size+margin)+margin, r*(tile_size+margin)+margin)
    return positions

def draw_animated_board(screen, board: tuple, font: pygame.font.Font, tile_size: int, margin: int, animated_tile: int = None, anim_pos: tuple = None):
    screen.fill(BACKGROUND)
    for r, row in enumerate(board):
        for c, val in enumerate(row):
            x, y = c*(tile_size+margin)+margin, r*(tile_size+margin)+margin
            # 跳过正在移动的方块和空格
            if val != 0 and val != animated_tile:
                # ============== 关键修改 2: 绘制逻辑 ==============
                if val == -1: # 如果是障碍物
                    pygame.draw.rect(screen, OBSTACLE_COLOR, (x, y, tile_size, tile_size), border_radius=8)
                else: # 如果是普通数字牌
                    pygame.draw.rect(screen, GREEN, (x, y, tile_size, tile_size), border_radius=8)
                    text = font.render(str(val), True, WHITE)
                    screen.blit(text, text.get_rect(center=(x + tile_size/2, y + tile_size/2)))
    # 单独绘制移动中的方块
    if animated_tile and anim_pos and animated_tile != -1:
        x, y = anim_pos
        pygame.draw.rect(screen, LIGHT_GREEN, (x, y, tile_size, tile_size), border_radius=8)
        text = font.render(str(animated_tile), True, WHITE)
        screen.blit(text, text.get_rect(center=(x + tile_size/2, y + tile_size/2)))

# --- 主函数 (不变) ---
def main():
    start_board, goal_board, puzzle_size = get_puzzles_from_console()
    if not start_board:
        input("\n按 Enter 键退出程序...")
        return

    print("\n正在调用 C++ 求解器...")
    solution_path = get_solution_path_from_cpp(start_board, goal_board)

    if puzzle_size <= 4: tile_size, font_size = 100, 50
    elif puzzle_size == 5: tile_size, font_size = 80, 40
    else: tile_size, font_size = 60, 30
    margin = 10
    grid_width = puzzle_size * (tile_size + margin) + margin
    width, height = grid_width, grid_width + 60

    pygame.init()
    screen = pygame.display.set_mode((width, height))
    pygame.display.set_caption(f"{puzzle_size**2 - 1}-Puzzle Solver")
    font = pygame.font.Font(None, font_size)
    small_font = pygame.font.Font(None, 32)
    clock = pygame.time.Clock()

    game_state = GameState.WAITING_FOR_START if solution_path else GameState.NO_SOLUTION
    current_step = 0
    is_animating, anim_start_time = False, 0
    animated_tile_val, anim_start_pos, anim_end_pos = None, (0, 0), (0, 0)
    
    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT: running = False
            if game_state == GameState.WAITING_FOR_START:
                if event.type == pygame.MOUSEBUTTONDOWN or event.type == pygame.KEYDOWN:
                    game_state = GameState.ANIMATING
            elif game_state in [GameState.NO_SOLUTION, GameState.FINISHED]:
                 if event.type == pygame.MOUSEBUTTONDOWN or event.type == pygame.KEYDOWN:
                     running = False
        screen.fill(BACKGROUND)
        if game_state == GameState.WAITING_FOR_START:
            draw_animated_board(screen, solution_path[0], font, tile_size, margin)
            msg_surf = small_font.render("点击任意处开始播放", True, BLUE_GRAY)
            screen.blit(msg_surf, (width/2 - msg_surf.get_width()/2, height - 45))
        elif game_state == GameState.ANIMATING or game_state == GameState.FINISHED:
            current_board = solution_path[current_step]
            if game_state == GameState.ANIMATING and not is_animating and current_step < len(solution_path) - 1:
                is_animating, anim_start_time = True, time.time()
                prev_b, next_b = solution_path[current_step], solution_path[current_step + 1]
                prev_p = get_tile_positions(prev_b, tile_size, margin)
                next_p = get_tile_positions(next_b, tile_size, margin)
                animated_tile_val = next(val for val in prev_p if val != -1 and prev_p[val] != next_p.get(val))
                anim_start_pos, anim_end_pos = prev_p[animated_tile_val], next_p[animated_tile_val]
            if is_animating:
                progress = min((time.time() - anim_start_time) / ANIMATION_DURATION, 1.0)
                curr_pos = (anim_start_pos[0] + (anim_end_pos[0] - anim_start_pos[0]) * progress,
                            anim_start_pos[1] + (anim_end_pos[1] - anim_start_pos[1]) * progress)
                draw_animated_board(screen, current_board, font, tile_size, margin, animated_tile_val, curr_pos)
                if progress >= 1.0:
                    is_animating, current_step = False, current_step + 1
                    if current_step == len(solution_path) - 1: game_state = GameState.FINISHED
            else:
                draw_animated_board(screen, current_board, font, tile_size, margin)
            step_text = f"Step: {current_step} / {len(solution_path) - 1}"
            msg_surf = small_font.render(step_text, True, BLUE_GRAY)
            screen.blit(msg_surf, (width/2 - msg_surf.get_width()/2, height - 45))
        elif game_state == GameState.NO_SOLUTION:
            msg_surf = font.render("此题无解", True, RED)
            msg_surf2 = small_font.render("点击任意处退出", True, BLUE_GRAY)
            screen.blit(msg_surf, (width/2 - msg_surf.get_width()/2, height/2 - msg_surf.get_height()))
            screen.blit(msg_surf2, (width/2 - msg_surf2.get_width()/2, height/2 + 10))
        pygame.display.flip()
        clock.tick(60)
    pygame.quit()

if __name__ == "__main__":
    main()