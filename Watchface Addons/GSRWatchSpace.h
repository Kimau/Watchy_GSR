// GSRWatchFaceSpace.h

RTC_DATA_ATTR int SpaceFaceStyle;	// Watchface ID #.


// #include "Watchy_GSR.h"

// Structure definitions
struct SpacePt {
    int16_t x;
    int16_t y;
    int16_t r;
};

struct SpaceXY {
    int x;
    int y;
};

// Must be here because it uses the structs above
#include "IconSpace.h"
#include "FontSunflower.h"
#include "FontMono.h"

class WatchySpaceFace : public WatchyGSR {
    
public:
    WatchySpaceFace() : WatchyGSR() {
        initAddOn(this);
    }

    void RegisterWatchFaces() {
        SpaceFaceStyle = AddWatchStyle("Space Face", this);
    }

    void InsertInitWatchStyle(uint8_t StyleID) {
        if (StyleID != SpaceFaceStyle) {
            return;
        }
        
        initializeDesignSettings();
    }

    void InsertDrawWatchStyle(uint8_t StyleID) {
        if (StyleID != SpaceFaceStyle) {
            return;
        }

        display.fillScreen(GxEPD_BLACK);
        
        if (SafeToDraw() && NoMenu()) {
            drawGrid();
            drawStars();
            drawISS();
        }
        
        drawTimeAndDate();
    }

private:
    // Constants
    const int HORIZON_Y = 150;
    const int PLANET_RADIUS = 20;
    const int VANISHING_POINT_OFFSET = 25;

    
    void drawBitmap1Bit(int16_t sx, int16_t sy, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color) 
    {
        int16_t offset = 0;
        uint8_t b = bitmap[offset];
        uint8_t bw = 0;

        display.startWrite();
        for (int16_t y = sy; y < (sy+h); y++) {
            for (int16_t x = sx; x < (sx+w); x++) {
                if(b & 0x80) {
                    display.writePixel(x, y, color);
                }
                
                b <<= 1;
                bw++;
                if(bw == 8) {
                    offset++;
                    bw = 0;
                    b = bitmap[offset];
                }
            }
        }
        display.endWrite();
    }
    
    /**
     * Draws a perspective grid with wavy horizontal lines
     */
    void drawGrid() {
        display.startWrite();

        // Horizontal lines using Fibonacci sequence
        int prevFib = 1;  // First Fibonacci number
        int currFib = 1;  // Second Fibonacci number
        int nextline = HORIZON_Y;

        for (int y = HORIZON_Y; y < 200; y++) {
            if (y == nextline) {
                display.writeLine(0, y, 200, y, GxEPD_BLACK);
                // Calculate next Fibonacci number and update line position
                int nextFib = prevFib + currFib;
                prevFib = currFib;
                currFib = nextFib;
                nextline = y + prevFib;  // Use Fibonacci number for spacing
            } else {
                display.writeLine(0, y, 200, y, GxEPD_WHITE);
            }
        }

        // Perspective lines
        int vanishingPointY = HORIZON_Y - VANISHING_POINT_OFFSET;
        for (int x = -230; x < 430; x += 20) {
            display.writeLine(x, 200, 100, vanishingPointY, GxEPD_BLACK);
        }
        display.endWrite();
    }

    /**
     * Draws rotating star field based on current minute
     */
    void drawStars() {
        double minuteAngle = ((2.0 * M_PI) / 60.0) * static_cast<double>(WatchTime.Local.Minute);
        
        for (int starI = 0; starI < STAR_COUNT; starI++) {
            const SpacePt& star = STAR_DATA[starI];
            SpaceXY rotated = rotatePointAround(star.x, star.y, 100, 100, minuteAngle);
            
            if (!isPointVisible(rotated)) {
                continue;
            }
            
            drawStar(rotated, star.r);
        }
    }

    void drawISS() {
        int xRange = 200 - ISS_WIDTH * 2;
        int t = int(WatchTime.Local.Minute * 60) + int(WatchTime.Local.Second);
        int tmax = 60 * 60;

        int xStart = xRange * t / tmax;
        drawBitmap1Bit(xStart, 10, iss, ISS_WIDTH, ISS_HEIGHT, 1);
    }
    
    /**
     * Checks if a point is within the visible area
     */
    bool isPointVisible(const SpaceXY& point) const {
        return point.x >= 0 && point.y >= 0 && 
               point.x <= 200 && point.y <= HORIZON_Y;
    }
    
    /**
     * Draws a single star
     */
    void drawStar(const SpaceXY& position, int radius) {
        if (radius == 0) {
            display.drawPixel(position.x, position.y, GxEPD_WHITE);
        } else {
            display.fillCircle(position.x, position.y, radius, GxEPD_WHITE);
        }
    }

    /**
     * Draws text centered at given coordinates with optional background
     */
    void drawCenteredString(const String& str, int x, int y, bool drawBackground = false) {
        int16_t x1, y1;
        uint16_t w, h;
        display.getTextBounds(str, x, y, &x1, &y1, &w, &h);
        
        if (drawBackground) {
            drawTextBackground(x, y, w, h);
        }
        
        display.setCursor(x - w / 2, y);
        display.print(str);
    }
    
    /**
     * Draws background rectangle for text
     */
    void drawTextBackground(int x, int y, uint16_t width, uint16_t height) {
        const int padY = 3;
        const int padX = 10;
        display.fillRect(
            x - (width / 2 + padX),
            y - (height + padY),
            width + padX * 2,
            height + padY * 2,
            GxEPD_BLACK
        );
    }
    
    /**
     * Rotates a point around an origin by given angle
     */
    SpaceXY rotatePointAround(int x, int y, int ox, int oy, double angle) {
        double cosAngle = cos(angle);
        double sinAngle = sin(angle);
        double dx = static_cast<double>(x - ox);
        double dy = static_cast<double>(y - oy);
        
        return {
            static_cast<int>(ox + (cosAngle * dx) + (sinAngle * dy)),
            static_cast<int>(oy + (-sinAngle * dx) + (cosAngle * dy))
        };
    }

    /**
     * Initializes all design-related settings
     */
    void initializeDesignSettings() {
        // Menu settings
        Design.Menu.Top = 56;
        Design.Menu.Header = 25;
        Design.Menu.Data = 66;
        Design.Menu.Gutter = 3;
        Design.Menu.Font = &aAntiCorona12pt7b;
        Design.Menu.FontSmall = &aAntiCorona11pt7b;
        Design.Menu.FontSmaller = &aAntiCorona10pt7b;
        
        // Face settings
        Design.Face.Bitmap = nullptr;
        Design.Face.SleepBitmap = nullptr;
        Design.Face.Gutter = 0;
        Design.Face.Time = 134;
        Design.Face.TimeHeight = 48;
        Design.Face.TimeColor = GSR_AutoFore;
        Design.Face.TimeFont = &MADE_Sunflower_PERSONAL_USE39pt7b;
        Design.Face.TimeLeft = 0;
        Design.Face.TimeStyle = WatchyGSR::dCENTER;
        
        initializeDateSettings();
        initializeStatusSettings();
    }
    
    /**
     * Initializes date-related design settings
     */
    void initializeDateSettings() {
        // Day settings
        Design.Face.Day = 44;
        Design.Face.DayGutter = 4;
        Design.Face.DayColor = GSR_AutoFore;
        Design.Face.DayFont = &FreeMonoBold9pt7b;
        Design.Face.DayFontSmall = &FreeMonoBold9pt7b;
        Design.Face.DayFontSmaller = &FreeMonoBold9pt7b;
        Design.Face.DayLeft = 0;
        Design.Face.DayStyle = WatchyGSR::dCENTER;
        
        // Date settings
        Design.Face.Date = 170;
        Design.Face.DateGutter = 4;
        Design.Face.DateColor = GSR_AutoFore;
        Design.Face.DateFont = &FreeMonoBold9pt7b;
        Design.Face.DateFontSmall = &FreeMonoBold9pt7b;
        Design.Face.DateFontSmaller = &FreeMonoBold9pt7b;
        Design.Face.DateLeft = 0;
        Design.Face.DateStyle = WatchyGSR::dLEFT;
        
        // Year settings
        Design.Face.Year = 170;
        Design.Face.YearLeft = 0;
        Design.Face.YearColor = GSR_AutoFore;
        Design.Face.YearFont = &FreeMonoBold9pt7b;
        Design.Face.YearStyle = WatchyGSR::dRIGHT;
    }
    
    /**
     * Initializes status-related design settings
     */
    void initializeStatusSettings() {
        Design.Status.WIFIx = 5;
        Design.Status.WIFIy = 193;
        Design.Status.BATTx = 155;
        Design.Status.BATTy = 178;
    }
    
    /**
     * Draws the time and date display
     */
    void drawTimeAndDate() {
        drawTime();
        drawDate();
    }
    
    /**
     * Draws the time display
     */
    void drawTime() {
        display.setFont(&MADE_Sunflower_PERSONAL_USE39pt7b);
        display.setTextColor(GxEPD_WHITE);
        String timeStr = MakeMinutes(WatchTime.Local.Hour) + ":" + 
                        MakeMinutes(WatchTime.Local.Minute);
        drawCenteredString(timeStr, 100, 115);
    }
    
    /**
     * Draws the date display
     */
    void drawDate() {
        display.setFont(&FreeMonoBold9pt7b);
        String dateStr = String(dayShortStr(WatchTime.Local.Wday + 1)) + " " +
                        String(monthShortStr(WatchTime.Local.Month + 1)) + " " +
                        String(WatchTime.Local.Day);
        drawCenteredString(dateStr, 100, 140, true);
    }
};

WatchySpaceFace watchySpaceFace;
