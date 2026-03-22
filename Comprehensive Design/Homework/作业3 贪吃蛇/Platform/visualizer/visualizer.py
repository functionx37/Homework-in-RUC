import pygame
import json
import sys
import os

CELL_SIZE = 24
MAP_WIDTH, MAP_HEIGHT = 40, 30
SCREEN_WIDTH, SCREEN_HEIGHT = MAP_WIDTH * CELL_SIZE, MAP_HEIGHT * CELL_SIZE + 60

COLOR_BACKGROUND = (20, 20, 30)
COLOR_GRID = (40, 40, 50)
COLOR_SAFE_ZONE = (0, 40, 0)
COLOR_SNAKE_BODY = [(50, 150, 50), (50, 50, 150), (150, 50, 50), (150, 150, 50), (50, 150, 150), (150, 50, 150)]
COLOR_SNAKE_HEAD = [(100, 255, 100), (100, 100, 255), (255, 100, 100), (255, 255, 100), (100, 255, 255), (255, 100, 255)]
COLOR_SHIELD = (200, 200, 255, 100)
FOOD_COLORS = {1: (255, 200, 0), 2: (255, 150, 0), 3: (255, 100, 0), 4: (255, 50, 0), 5: (255, 0, 0)}
COLOR_TRAP = (200, 0, 255)
COLOR_GROW = (0, 255, 255)
COLOR_KEY = (255, 223, 0)
COLOR_CHEST = (139, 69, 19)
COLOR_UI_TEXT = (220, 220, 220)

def draw_frame(screen, font, game_tick_data, total_ticks, paused):
    screen.fill(COLOR_BACKGROUND)
    sz = game_tick_data.get('safe_zone', {})
    pygame.draw.rect(screen, COLOR_SAFE_ZONE, (sz.get('xmin',0)*CELL_SIZE, sz.get('ymin',0)*CELL_SIZE, (sz.get('xmax',39)-sz.get('xmin',0)+1)*CELL_SIZE, (sz.get('ymax',29)-sz.get('ymin',0)+1)*CELL_SIZE))
    for x in range(0, SCREEN_WIDTH, CELL_SIZE): pygame.draw.line(screen, COLOR_GRID, (x, 0), (x, MAP_HEIGHT*CELL_SIZE))
    for y in range(0, MAP_HEIGHT*CELL_SIZE + 1, CELL_SIZE): pygame.draw.line(screen, COLOR_GRID, (0, y), (SCREEN_WIDTH, y))

    # ItemType: 0:Food, 1:GrowBean, 2:Trap
    for item in game_tick_data.get('items', []):
        pos, item_type, value = item['pos'], item['type'], item['value']
        color = COLOR_GRID
        if item_type == 0: color = FOOD_COLORS.get(value, (255,255,255))
        elif item_type == 1: color = COLOR_GROW
        elif item_type == 2: color = COLOR_TRAP
        pygame.draw.rect(screen, color, (pos[1]*CELL_SIZE+4, pos[0]*CELL_SIZE+4, CELL_SIZE-8, CELL_SIZE-8))
    
    for key in game_tick_data.get('keys', []):
        pos = key['pos']
        pygame.draw.circle(screen, COLOR_KEY, (pos[1]*CELL_SIZE+CELL_SIZE//2, pos[0]*CELL_SIZE+CELL_SIZE//2), CELL_SIZE//3)
    
    for chest in game_tick_data.get('chests', []):
        pos = chest['pos']
        pygame.draw.rect(screen, COLOR_CHEST, (pos[1]*CELL_SIZE+2, pos[0]*CELL_SIZE+2, CELL_SIZE-4, CELL_SIZE-4))
        pygame.draw.rect(screen, COLOR_KEY, (pos[1]*CELL_SIZE+8, pos[0]*CELL_SIZE, CELL_SIZE-16, CELL_SIZE//4))
    
    for i, s in enumerate(game_tick_data.get('snakes', [])):
        color_idx = s['id'] % len(COLOR_SNAKE_BODY)
        for part in s['body'][1:]: pygame.draw.rect(screen, COLOR_SNAKE_BODY[color_idx], (part[1]*CELL_SIZE, part[0]*CELL_SIZE, CELL_SIZE, CELL_SIZE))
        if s['body']:
            head = s['body'][0]
            pygame.draw.rect(screen, COLOR_SNAKE_HEAD[color_idx], (head[1]*CELL_SIZE, head[0]*CELL_SIZE, CELL_SIZE, CELL_SIZE))
            if s['has_key']: pygame.draw.circle(screen, COLOR_KEY, (head[1]*CELL_SIZE+CELL_SIZE//2, head[0]*CELL_SIZE+CELL_SIZE//2), CELL_SIZE//4)
            if s['has_shield']:
                shield_surface = pygame.Surface((CELL_SIZE,CELL_SIZE), pygame.SRCALPHA); shield_surface.fill(COLOR_SHIELD); screen.blit(shield_surface, (head[1]*CELL_SIZE, head[0]*CELL_SIZE))

    ui_panel_y = MAP_HEIGHT * CELL_SIZE
    pygame.draw.rect(screen, (50,50,60), (0, ui_panel_y, SCREEN_WIDTH, 60))
    status_text = "[❚❚ PAUSED]" if paused else "[▶ PLAYING]"; status_surface = font.render(status_text, True, (255, 200, 0)); screen.blit(status_surface, (10, ui_panel_y + 35))
    tick_text = f"Tick: {game_tick_data.get('tick', 0)} / {total_ticks}"; tick_surface = font.render(tick_text, True, COLOR_UI_TEXT); screen.blit(tick_surface, (10, ui_panel_y + 10))
    x_offset = 150
    for i, s in enumerate(game_tick_data.get('snakes', [])):
        color_idx = s['id'] % len(COLOR_SNAKE_HEAD)
        score_text = f"P{i}(ID:{s['id']}): {s['score']}"; score_surface = font.render(score_text, True, COLOR_SNAKE_HEAD[color_idx]); screen.blit(score_surface, (x_offset, ui_panel_y + 10 + (i%2 * 25) ))
        if i % 2 == 1: x_offset += 250
    pygame.display.flip()

def main():
    if len(sys.argv) < 2: print("Usage: python visualizer.py <game.log>"); return
    log_path = sys.argv[1]
    if not os.path.exists(log_path) or os.path.getsize(log_path) == 0: print(f"Error: Log file not found or empty: {log_path}"); return
    all_ticks = [json.loads(line) for line in open(log_path, 'r') if line.strip()]
    if not all_ticks: print("Log file is empty or invalid."); return
    
    pygame.init(); pygame.font.init()
    screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
    pygame.display.set_caption("AI Snake - (Space: Pause/Play, R: Restart)")
    clock = pygame.time.Clock(); font = pygame.font.Font(None, 24)
    try: font = pygame.font.SysFont("SimHei", 20)
    except: pass
    
    idx, running, paused = 0, True, True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT: running = False
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE: paused = not paused
                elif event.key == pygame.K_r: idx, paused = 0, False
                elif event.key == pygame.K_RIGHT: idx = min(len(all_ticks)-1, idx+1); paused = True
                elif event.key == pygame.K_LEFT: idx = max(0, idx-1); paused = True
        
        draw_frame(screen, font, all_ticks[idx], len(all_ticks)-1, paused)
        if not paused:
            if idx < len(all_ticks) - 1: idx += 1
            else: paused = True
        clock.tick(5)
    pygame.quit()

if __name__ == '__main__': main()