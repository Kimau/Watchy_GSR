// GSRWatchFaceSpace.h

RTC_DATA_ATTR int MST3KFaceStyle;	// Watchface ID #.


// #include "Watchy_GSR.h"

// Must be here because it uses the structs above
#include "IconMST3K.h"
#include "FontSunflower.h"
#include "FontMono.h"
#include "FontSans9pt7b.h"

// Lines
#define HOST_QUOTE_COUNT 20
static const char* host_quotes[] = {
    "You know what would improve this movie? Literally anything else",
    "I'm pretty sure this cinematographer just discovered zoom for the first time",
    "The special effects budget was clearly spent on coffee instead",
    "Well, that's 90 minutes of my life I'm never getting back",
    "I think this movie was written by a random plot generator",
    "The director must have gotten his degree from the back of a cereal box",
    "I've seen better acting in furniture commercials",
    "This makes Plan 9 look like Citizen Kane",
    "I think the monster is just three kids in a trench coat",
    "The boom mic should get top billing in this scene",
    "Is this what happens when you let your screensaver direct a movie?",
    "Pretty sure this set was someone's garage last week",
    "The continuity editor must have been on vacation",
    "I've seen better special effects in flip books",
    "Did they film this with a potato?",
    "The script must have been written on a napkin",
    "This makes interpretive dance look straightforward",
    "I think this monster was made with paper mache and optimism",
    "The plot holes are big enough to drive a monster truck through",
    "This dialogue sounds like it was translated to Japanese and back",
    "I'm convinced this was edited with safety scissors",
    "The costume department raided a Halloween clearance sale",
    "This is what happens when you let your cat write the screenplay",
    "The tracking on this movie needs therapy",
    "Pretty sure this was shot entirely during lunch breaks",
    "The aspect ratio is having an identity crisis",
    "This film stock looks like it was developed in coffee",
    "The sound mixing was done by throwing darts at a mixer",
    "I think they bought these props at a yard sale",
    "The director clearly went to the 'point and pray' school of filmmaking"
};

#define TOM_QUOTE_COUNT 20
static const char* tom_quotes[] = {
    "That explosion looks like someone threw confetti at the camera",
    "The monster appears to be wearing tube socks",
    "I think those are just Christmas lights painted green",
    "That spaceship is definitely a hubcap on a string",
    "The alien's antennae are bent pipe cleaners",
    "This chase scene was clearly filmed in someone's driveway",
    "That laser sound is just someone saying 'pew pew'",
    "The robot is wearing a colander for a helmet",
    "Those mountains in the background are painted on a bedsheet",
    "The alien spacecraft interior is just cardboard boxes painted silver",
    "That futuristic computer is a microwave with LEDs glued on",
    "The time machine looks suspiciously like a phone booth",
    "Those are just ping pong balls painted to look like eyes",
    "The death ray is clearly a modified hair dryer",
    "That alien language sounds like pig Latin",
    "The force field is just cellophane wrapped around fishing line",
    "Those alien costumes are just painted rain ponchos",
    "The control panel is decorated with bottle caps",
    "That's not a meteor, that's a painted bowling ball",
    "The monster's scales are cut up rubber mats",
    "Their ray guns are painted super soakers",
    "The alien city is made of upside-down paper cups",
    "That's not anti-gravity, those are just visible wires",
    "The monster's roar sounds like a slowed-down duck",
    "Those tentacles are painted garden hoses",
    "The flying saucer is spinning on a ceiling fan motor",
    "That alien goo is just green jello",
    "Their space helmets are repurposed fish bowls",
    "The creature's eyes are ping pong balls with dots",
    "That's not a portal, it's a hula hoop with tinfoil"
};

#define CROW_QUOTE_COUNT 20
static const char* crow_quotes[] = {
    "The editing in this scene gave me whiplash",
    "I've seen better green screens in weather forecasts",
    "That stunt double isn't even wearing the same color shirt",
    "Did they record this audio underwater?",
    "The director must have been filming his own shadow",
    "Those aliens look like rejected Muppet designs",
    "The spaceship interior is someone's wood-paneled basement",
    "Pretty sure that monster is just a guy in garbage bags",
    "This chase scene was filmed at 2 miles per hour",
    "The alien planet looks like someone's rock garden",
    "That futuristic city is made of bathroom accessories",
    "The time portal is a shower curtain with glitter",
    "Those robot movements are just someone doing the robot",
    "The alien queen is wearing a lampshade crown",
    "This fight scene looks like interpretive dance",
    "The earthquake effect is just shaking the camera",
    "That alien technology is painted kitchen utensils",
    "The monster's lair is decorated with dryer lint",
    "Those space boots are just painted rain boots",
    "The alien writing is backwards alphabet soup",
    "That mind control device is a painted salad bowl",
    "The mutant is wearing modified swimming goggles",
    "Those are just party streamers for energy beams",
    "The alien throne is a decorated lawn chair",
    "That radar screen is a painted record player",
    "The monster's teeth are painted popsicle sticks",
    "Their jet packs are painted fire extinguishers",
    "That's not a forcefield, it's bubble wrap",
    "The alien database is an old calculator display",
    "Those meteor craters are just painted kiddie pools"
};


class WatchyMST3KFace : public WatchyGSR {
    
public:
    WatchyMST3KFace() : WatchyGSR() {
        initAddOn(this);
    }

    void RegisterWatchFaces() {
        MST3KFaceStyle = AddWatchStyle("MST3K Face", this);
    }

    void InsertInitWatchStyle(uint8_t StyleID) {
        if (StyleID != MST3KFaceStyle) {
            return;
        }
        
        initializeDesignSettings();
    }

    int quoteId = -1;

    void InsertDrawWatchStyle(uint8_t StyleID) {
        if (StyleID != MST3KFaceStyle) {
            return;
        }

        display.fillScreen(GxEPD_WHITE);

        if((quoteId < 0) || (WatchTime.NewMinute)) {
            quoteId = esp_random() % (HOST_QUOTE_COUNT + TOM_QUOTE_COUNT + CROW_QUOTE_COUNT);
        }
        
        if (SafeToDraw()) { // && NoMenu()

            // use the minute to make a bitmask for which characters are moving up and down
            int bm = WatchTime.Local.Hour;
            int sm = WatchTime.Local.Minute;

            int tomX = 28;
            int tomY = 200 - TOM_HEIGHT;
            tomX += sin(bm * 2 * M_PI / 60.0) * 10;
            if (bm & 0x1) {
                tomY -= sin(sm * M_PI / 60.0) * 12;
            }

            int joY = 200 - JO_HEIGHT;
            if (bm & 0x2) {
                joY -= sin(sm * M_PI / 60.0) * 5;
            }

            int crowY = 200 - CROW_HEIGHT;
            if (bm & 0x4) {
                crowY -= sin(sm * M_PI / 60.0) * 6;
            }

            // Draw Characters
            drawBitmap2Bit(tomX, tomY, TOM, TOM_WIDTH, TOM_HEIGHT);
            drawBitmap2Bit(75, joY, JO, JO_WIDTH, JO_HEIGHT);
            drawBitmap2Bit(150, crowY, CROW, CROW_WIDTH, CROW_HEIGHT);

            // Draw Seats
            drawBitmap2Bit(0, 200 - SEATS_HEIGHT, SEATS, SEATS_WIDTH, SEATS_HEIGHT);

            // Draw Quote
            if(quoteId < HOST_QUOTE_COUNT) {
                drawChatBubble(host_quotes[quoteId], 2,2);
            } else if(quoteId < HOST_QUOTE_COUNT + TOM_QUOTE_COUNT) {
                drawChatBubble(tom_quotes[quoteId - HOST_QUOTE_COUNT], 2,2);
            } else {
                drawChatBubble(crow_quotes[quoteId - HOST_QUOTE_COUNT - TOM_QUOTE_COUNT], 2,2);
            }

            // Draw Time and Date
            drawTimeAndDate();
        }
        
    }

private:
    
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

    void drawBitmap2Bit(int16_t sx, int16_t sy, const uint8_t bitmap[], int16_t w, int16_t h) 
    {
        int16_t offset = 0;
        uint8_t b = bitmap[offset];
        uint8_t bw = 0;

        display.startWrite();
        for (int16_t y = sy; y < (sy+h); y++) {
            for (int16_t x = sx; x < (sx+w); x++) {
                if(b & 0x80) {
                    if (b & 0x40) {
                        display.writePixel(x, y, GxEPD_BLACK);
                    } else {
                        display.writePixel(x, y, GxEPD_WHITE);
                    }
                }
                
                b <<= 2;
                bw += 2;
                if(bw == 8) {
                    offset++;
                    bw = 0;
                    b = bitmap[offset];
                }
            }
        }
        display.endWrite();
    }

    void drawChatBubble(const String& text, int x, int y) {
        const int GUTTER = 1;
        const int BORDER = 1;
        const int PADDING = GUTTER + BORDER;
        const int MAX_WIDTH = 200;

        display.setFont(&FreeSans9pt7b);
        display.setTextColor(GxEPD_BLACK);
        
        // Calculate text wrapping
        int16_t x1, y1;
        uint16_t w1, h1;
        display.getTextBounds(text, 0, 0, &x1, &y1, &w1, &h1);

        if(w1 > MAX_WIDTH) {
            // How wide is the text vs the max width?
            int num_lines = w1 / MAX_WIDTH + 1;

            int ys = 5;

            String remainText = text;

            // Draw a line. Div remaining text by remain lines. Find the next space. Split the line. Get the text bounds. set bw to max(bw, w1). set cursor to w1/2
            while(remainText.length() > 0) {
                int tl = remainText.length() / num_lines;
                int sp = remainText.indexOf(' ', tl);
                if ((sp >=0) && (sp < (tl+5))) {
                    tl = sp;
                }

                String line = remainText.substring(0, tl);
                display.getTextBounds(line, 0, 0, &x1, &y1, &w1, &h1);

                display.setCursor((MAX_WIDTH - w1) / 2, ys+h1);
                display.print(line);
                remainText = remainText.substring(line.length());
                ys += h1;
                num_lines--;
            }
        } else {
            display.setCursor((MAX_WIDTH - w1) / 2, 10+h1);
            display.print(text);
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
            GxEPD_WHITE
        );
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
        Design.Status.WIFIy = 40;
        Design.Status.BATTx = 155;
        Design.Status.BATTy = 40;
    }
    
    /**
     * Draws the time and date display
     */
    void drawTimeAndDate() {
        
        display.setFont(&MADE_Sunflower_PERSONAL_USE39pt7b);
        String timeStr = MakeMinutes(WatchTime.Local.Hour) + ":" + 
                        MakeMinutes(WatchTime.Local.Minute);

        display.setTextColor(GxEPD_WHITE);
        drawCenteredString(timeStr, 98, 115);
        drawCenteredString(timeStr, 102, 115);

        display.setTextColor(GxEPD_BLACK);
        drawCenteredString(timeStr, 100, 115);

        display.setFont(&FreeMonoBold9pt7b);
        String dateStr = String(dayShortStr(WatchTime.Local.Wday + 1)) + " " +
                        String(monthShortStr(WatchTime.Local.Month + 1)) + " " +
                        String(WatchTime.Local.Day);
        drawCenteredString(dateStr, 100, 140, true);
    }
};

WatchyMST3KFace watchyMST3KFace;
