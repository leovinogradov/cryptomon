import pygame
from option import Option
from option import OptionWheel

class PygameView(object):


    def __init__(self, width=480, height=320, fps=30):
        """Initialize pygame, window, background, font,...
        """
        pygame.init()
        pygame.display.set_caption("Press ESC to quit")
        self.width = width
        self.height = height
        self.screen = pygame.display.set_mode((self.width, self.height), pygame.DOUBLEBUF)
        self.background = pygame.Surface(self.screen.get_size()).convert_alpha()
        self.clock = pygame.time.Clock()
        self.fps = fps
        self.playtime = 0.0
        self.font = pygame.font.SysFont('mono', 20, bold=True)

        self.init_main_screen()


    def run(self):
        running = True
        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_ESCAPE:
                        running = False
                    if event.key == pygame.K_RIGHT:
                        self.opWheel.scroll_right()
                    if event.key == pygame.K_DOWN:
                        self.opWheel.select()
                    if event.key == pygame.K_LEFT:
                        self.opWheel.scroll_left()
            self.opWheel.draw_op_wheel()
            pygame.display.flip()
            self.screen.blit(self.background, (0, 0))

        pygame.quit()

    def init_main_screen(self):
        self.background = pygame.image.load("Stage.jpg")
        self.opWheel = OptionWheel(self)
        
    def draw_FPS(self):
        milliseconds = self.clock.tick(self.fps)
        self.playtime += milliseconds / 1000.0
        self.draw_text("FPS: {:6.3}{}PLAYTIME: {:6.3} SECONDS".format(
            self.clock.get_fps(), " "*5, self.playtime),0,0)

    def draw_text(self, text, x, y):
        surface = self.font.render(text, True, (0, 255, 0))
        self.screen.blit(surface, (x, y))

    def draw_text_center(self, text):
        fw, fh = self.font.size(text) # fw: font width,  fh: font height
        surface = self.font.render(text, True, (0, 255, 0))
        # // makes integer division in python3
        self.screen.blit(surface, ((self.width - fw) // 2, (self.height - fh) // 2))

####

if __name__ == '__main__':

    # call with width of window and fps
    PygameView(480, 320).run()
