# 🪪📜 MIFARE Poetry

**An ephemeral micropoetry format that uses MIFARE Classic NFC cards as the literary medium: three lines, up to 16 characters per line, and only three readings before the poem disappears.**

MIFARE Poetry explores what happens when the physical limitations of a storage technology become the formal rules of a literary genre.

The project uses an **Arduino**, an **MFRC522 RFID/NFC reader**, an OLED display, MIFARE Classic 1K cards, a small PHP web interface, and optional Python poetry generators.

Instead of treating an NFC card only as an identifier or payment token, the card becomes the poem itself.

```text
Writer
  │
  ▼
Web interface
  │
  ▼
mifare.json
  │
  ▼
Wi-Fi
  │
  ▼
Arduino UNO R4 WiFi
  │
  ▼
MFRC522
  │
  ▼
MIFARE Classic 1K card
  │
  ├── Block 4 → Line 1
  ├── Block 5 → Line 2
  ├── Block 6 → Line 3
  └── Block 8 → Read counter
```

When the device is switched to read mode:

```text
Card presented
      │
      ▼
Read three lines
      │
      ▼
Display poem
      │
      ▼
Decrease counter
      │
      ▼
Counter reaches 0?
      │
   ┌──┴───┐
   │      │
  No     Yes
   │      │
   ▼      ▼
 Keep    Delete
 poem    poem
```

The result is a deliberately fragile form of electronic literature.

---

## ✨ Features

- 🪪 MIFARE Classic 1K cards as the literary medium
- 📜 Three-line micropoetry format
- 🔢 Maximum of 16 characters per line
- ⏳ Maximum of three readings per poem
- 🗑️ Automatic deletion after the final reading
- ⚡ Arduino UNO R4 WiFi
- 📡 MFRC522 13.56 MHz reader/writer
- 🖥️ OLED display
- 🌐 Web-based read/write configuration
- 📄 Simple JSON communication between server and Arduino
- 🐍 Automatic Python poetry generator
- 🎵 Optional rhyming generator
- 💾 Poetry stored directly on the NFC card
- 🔑 MIFARE sector authentication
- 🖨️ Custom two-piece 3D-printed enclosure
- 🎭 Electronic literature / media-art experiment
- 📜 MIT licensed

---

# 📐 The rules of MIFARE Poetry

MIFARE Poetry has three rules.

## Rule #1

**Each line can contain a maximum of 16 characters, including spaces.**

```text
1234567890123456
```

This limit comes directly from the size of a MIFARE Classic memory block:

```text
16 bytes
```

For simple ASCII text, this provides room for:

```text
16 characters
```

per block.

---

## Rule #2

**Every poem contains exactly three lines.**

Example:

```text
Waves crash
time bends
echoes fade
```

Each line occupies a separate MIFARE memory block.

---

## Rule #3

**Every poem can only be read three times.**

A separate memory block stores the remaining-read counter.

Each successful reading changes:

```text
3
↓
2
↓
1
↓
0
```

At zero, the poem is erased.

The card remains reusable, but that particular poem no longer exists on it.

---

# 📜 Example poems

```text
Neon dreams glow
wires hum softly
code whispers life
```

```text
Waves crash
time bends
echoes fade
```

```text
releasing coil
belligerent mass
lake, vest
```

The format does not impose:

- syllable counts
- meter
- rhyme
- punctuation
- language
- subject

Its structure comes from the storage medium itself.

---

# 💾 MIFARE Classic 1K

The project is built around **MIFARE Classic 1K** cards and tags.

MIFARE Classic operates at:

```text
13.56 MHz
```

and uses passive RFID technology.

The card does not require a battery.

```text
Reader electromagnetic field
            │
            ▼
        Card coil
            │
            ▼
      Card powered
            │
            ▼
     Data exchange
```

A MIFARE Classic 1K card contains:

```text
1 KB EEPROM
16 sectors
4 blocks per sector
16 bytes per block
```

Conceptually:

```text
Sector 0
├── Block 0
├── Block 1
├── Block 2
└── Block 3  ← Sector Trailer

Sector 1
├── Block 4
├── Block 5
├── Block 6
└── Block 7  ← Sector Trailer

Sector 2
├── Block 8
├── Block 9
├── Block 10
└── Block 11 ← Sector Trailer

...
```

The last block of each sector is the:

```text
Sector Trailer
```

and contains:

- authentication keys
- access conditions

It should not be treated as ordinary text storage.

---

# 🗺️ MIFARE Poetry memory map

The reference implementation stores the poem in:

| Block | Purpose |
|---:|---|
| `4` | Line 1 |
| `5` | Line 2 |
| `6` | Line 3 |
| `7` | Sector Trailer — not used for poetry |
| `8` | Remaining-read counter |

```text
MIFARE Classic 1K

Sector 1
┌──────────────────────────┐
│ Block 4 → Poetry line 1  │
├──────────────────────────┤
│ Block 5 → Poetry line 2  │
├──────────────────────────┤
│ Block 6 → Poetry line 3  │
├──────────────────────────┤
│ Block 7 → Sector Trailer │
└──────────────────────────┘

Sector 2
┌──────────────────────────┐
│ Block 8 → Read counter   │
└──────────────────────────┘
```

Other data blocks can be selected by modifying the Arduino source.

---

# 🔐 Sector authentication

MIFARE Classic sectors are protected by keys and access conditions.

A factory/default card commonly uses:

```text
FF FF FF FF FF FF
```

as Key A.

The Arduino must authenticate the relevant sector before reading or writing protected blocks.

The reference project is intended for:

```text
blank cards
development cards
cards owned by the user
```

rather than modifying third-party transport, payment, access-control, or identification cards.

---

# 🧠 The storage medium defines the poem

The 16-character rule is not an arbitrary literary restriction.

It emerges from:

```text
MIFARE block size
        │
        ▼
16 bytes
        │
        ▼
16 ASCII characters
        │
        ▼
maximum line length
```

Likewise, the three-line rule maps naturally to the three usable data blocks of one standard MIFARE sector:

```text
Block 4
Block 5
Block 6
```

The physical memory layout therefore becomes a poetic constraint.

---

# ⏳ Ephemeral storage

Most digital technology attempts to preserve information indefinitely:

```text
backups
cloud storage
archives
version history
replication
```

MIFARE Poetry intentionally moves in the opposite direction.

```text
Write
  │
  ▼
Read
  │
  ▼
Read
  │
  ▼
Read
  │
  ▼
Erase
```

The disappearance is implemented in software, but the literary object is physical.

Once the counter reaches zero, presenting the same card no longer reveals the original poem.

---

# 🧰 Hardware

| Qty | Component | Purpose |
|---:|---|---|
| 1 | **Arduino UNO R4 WiFi** | Main controller and network connection |
| 1 | **MFRC522 / RC522 module** | MIFARE reader/writer |
| 1 | **OLED display** | Poetry display |
| 1+ | **MIFARE Classic 1K cards or tags** | Poetry storage |
| 1 | USB-C cable / power supply | Power and programming |
| 1 | 3D-printed enclosure | Device body |

The current repository documentation specifies an:

```text
Adafruit OLED 128 × 32
```

for the display.

---

# ⚡ Arduino UNO R4 WiFi

The project uses the **Arduino UNO R4 WiFi**.

The board combines:

```text
Renesas RA4M1
+
ESP32-S3 connectivity module
```

Relevant specifications include:

- Arm Cortex-M4
- 48 MHz
- 256 KB Flash
- 32 KB SRAM
- Wi-Fi
- Bluetooth
- 12×8 onboard LED matrix
- USB-C
- Qwiic connector
- traditional UNO form factor

Official documentation:

**[Arduino UNO R4 WiFi](https://docs.arduino.cc/hardware/uno-r4-wifi/)**

---

# 📡 MFRC522

The **MFRC522** is a 13.56 MHz contactless reader/writer IC supporting ISO/IEC 14443 A / MIFARE communication.

The inexpensive RC522 modules commonly used with Arduino expose an SPI interface.

```text
Arduino UNO R4
      │
      │ SPI
      ▼
   MFRC522
      │
      │ 13.56 MHz
      ▼
 MIFARE card
```

NXP documentation:

**[MFRC522 datasheet](https://www.nxp.com/docs/en/data-sheet/MFRC522.pdf)**

---

# 🔌 Wiring

## MFRC522 → Arduino UNO R4 WiFi

The reference project uses:

| MFRC522 | Arduino |
|---|---:|
| `SDA / SS` | `D10` |
| `SCK` | `D13` |
| `MOSI` | `D11` |
| `MISO` | `D12` |
| `RST` | `D9` |
| `GND` | `GND` |
| `3.3V` | `3.3V` |

```text
MFRC522              UNO R4 WiFi

SDA / SS  ─────────► D10
SCK       ─────────► D13
MOSI      ─────────► D11
MISO      ─────────► D12
RST       ─────────► D9
GND       ─────────► GND
3.3V      ─────────► 3.3V
```

### Important

The MFRC522 is a:

```text
3.3 V device
```

and should be powered from the Arduino's 3.3 V output.

---

# 🖥️ OLED wiring

The display communicates using I²C.

| OLED | Arduino |
|---|---|
| `VCC` | Supply |
| `GND` | `GND` |
| `SDA` | `SDA` |
| `SCL` | `SCL` |

```text
OLED
 │
 ├── SDA ───── SDA
 ├── SCL ───── SCL
 ├── VCC ───── VCC
 └── GND ───── GND
```

The OLED displays the poem after a successful read operation.

The complete poem is also available through the Arduino Serial Monitor during development.

---

# 🌐 Web interface

The device connects to Wi-Fi and obtains its operating instructions from a simple web-based system.

The server controls:

```text
mode
line 1
line 2
line 3
```

through a JSON file.

Example:

```json
{
  "mode": "r",
  "line1": "Hi there",
  "line2": "how are you",
  "line3": "ok"
}
```

The project therefore uses a deliberately simple architecture:

```text
Browser
   │
   ▼
PHP page
   │
   ▼
mifare.json
   │
   ▼
HTTP
   │
   ▼
UNO R4 WiFi
```

---

# ✍️ Write mode

When the web interface is placed in write mode:

```text
Browser
   │
   ▼
Enter three lines
   │
   ▼
PHP
   │
   ▼
mifare.json
   │
   ▼
Arduino downloads poem
   │
   ▼
Present MIFARE card
   │
   ▼
Write blocks 4, 5, 6
   │
   ▼
Reset read counter
```

The poem must respect:

```text
line 1 ≤ 16 characters
line 2 ≤ 16 characters
line 3 ≤ 16 characters
```

---

# 📖 Read mode

When the server configuration selects read mode:

```text
Present card
      │
      ▼
Authenticate sectors
      │
      ▼
Read blocks 4, 5, 6
      │
      ▼
Read block 8 counter
      │
      ▼
Display poem
      │
      ▼
Decrease counter
```

When the reading allowance is exhausted:

```text
counter == 0
```

the stored poem is deleted.

The physical card can then be reused for another poem.

---

# 🔄 Read/write workflow

```text
                     WEB SERVER
               ┌──────────────────┐
               │ mode: r / w      │
               │ line1            │
               │ line2            │
               │ line3            │
               └────────┬─────────┘
                        │
                       Wi-Fi
                        │
                        ▼
               ┌──────────────────┐
               │ Arduino UNO R4   │
               │      WiFi        │
               └────────┬─────────┘
                        │
                 ┌──────┴──────┐
                 │             │
             WRITE MODE     READ MODE
                 │             │
                 ▼             ▼
             MIFARE          MIFARE
              card            card
                 │             │
                 ▼             ▼
           Store poem      Read poem
           Counter = 3         │
                               ▼
                            OLED
                               │
                               ▼
                       Counter - 1
                               │
                               ▼
                       Delete at zero
```

---

# 🐍 Automatic poetry generation

The repository also includes Python generators that create poems automatically while respecting the MIFARE format.

Current repository structure includes:

```text
generator/
├── mifarepoetry1.py
└── mifarepoetry2.py
```

The project documentation describes them as:

```text
Standard version
Rhymed version
```

---

# 🎲 Generator version 1

The first generator combines words using:

```text
random selection
+
templates
+
length validation
```

The important rule is preserved programmatically:

```text
len(line) <= 16
```

Instead of generating arbitrary text and truncating it afterward, the generator selects words until it obtains a phrase that fits inside a MIFARE block.

Conceptually:

```text
word lists
   │
   ▼
template
   │
   ▼
candidate line
   │
   ▼
length <= 16?
   │
 ┌─┴──┐
 │    │
yes   no
 │    │
 ▼    └── generate again
line
```

---

# 🎵 Generator version 2 — rhyme

`generator/mifarepoetry2.py` extends the generator with rhyme.

The current source loads:

```text
nouns.txt
adjectives.txt
verbs.txt
adverbs.txt
gerunds.txt
```

and imports:

```python
from rhyme import rhymes_with
```

The program then builds three lines.

---

## Line 1

The first line randomly chooses among structures including:

```text
My <adjective> <noun>
Your <noun>
<adjective> <noun>
<noun>, <noun>
```

The resulting line is regenerated until:

```python
len(myLine) <= 16
```

The final noun is retained as the word to rhyme with later.

---

## Line 2

The second line uses:

```text
<verb> <adverb>
```

and repeats the selection until the complete phrase fits within:

```text
16 characters
```

---

## Line 3

The third line obtains words rhyming with the noun selected in line 1.

Its structure is:

```text
<gerund> <rhyming noun>
```

Again:

```python
len(myLine) <= 16
```

is enforced.

The resulting structure is therefore approximately:

```text
Line 1 → establishes noun
Line 2 → verb + adverb
Line 3 → gerund + rhyme(Line 1)
```

---

# 🎼 Rhyming Dictionary

The rhymed generator uses the Python `rhyme` module and the **Rhyming Dictionary** project.

The source notes:

```text
pip install pyrhyme
```

The accompanying Medium documentation also references the Rhyming Dictionary database tools.

Reference:

**[Rhyming Dictionary](https://rhyme.sourceforge.net/)**

---

# 🧪 Example generated poem

A generated poem may look like:

```text
nostalgic fuel
half cake
sniff zestily
```

or:

```text
releasing coil
belligerent mass
lake, vest
```

The generator is intentionally constrained and combinatorial.

It does not need a language model to produce valid MIFARE poems.

---

# 📁 Repository structure

The current repository is organized into three main functional areas:

```text
MIFAREPoetry/
├── arduino/
│   └── Arduino device source
│
├── generator/
│   ├── mifarepoetry1.py
│   ├── mifarepoetry2.py
│   └── word-list resources
│
├── web/
│   └── PHP / web interface files
│
├── LICENSE
└── README.md
```

### `arduino/`

Firmware for the:

```text
Arduino UNO R4 WiFi
+
MFRC522
+
OLED
```

reader/writer device.

### `generator/`

Python programs and word resources for automatically generating poems within the 16-character constraint.

### `web/`

Web/PHP components used to select:

```text
read mode
write mode
poetry text
```

and publish the configuration consumed by the Arduino.

---

# 🚀 Installation

## 1. Clone the repository

```bash
git clone https://github.com/ronibandini/MIFAREPoetry.git
cd MIFAREPoetry
```

---

## 2. Install Arduino IDE

Download:

**[Arduino IDE](https://www.arduino.cc/en/software)**

---

## 3. Install UNO R4 board support

From Arduino IDE:

```text
Tools
→ Board
→ Boards Manager
```

install:

```text
Arduino UNO R4 Boards
```

Then select:

```text
Arduino UNO R4 WiFi
```

---

## 4. Install the MFRC522 library

Install an Arduino MFRC522 library compatible with the sketch.

A widely used implementation is:

**[MFRC522 Arduino library](https://github.com/miguelbalboa/rfid)**

The library provides functions for:

- detecting cards
- reading the UID
- authenticating sectors
- reading MIFARE blocks
- writing MIFARE blocks

---

## 5. Install the OLED libraries

Install the display libraries required by the Arduino source.

Typical Adafruit OLED projects use:

```text
Adafruit GFX
Adafruit SSD1306
```

Check the current sketch includes before compiling and install the corresponding versions through Arduino Library Manager.

---

## 6. Connect the MFRC522

Use:

```text
SDA/SS → D10
SCK    → D13
MOSI   → D11
MISO   → D12
RST    → D9
3.3V   → 3.3V
GND    → GND
```

---

## 7. Connect the OLED

Use the board's I²C connections:

```text
SDA → SDA
SCL → SCL
```

plus power and ground.

---

## 8. Configure Wi-Fi

Set the Wi-Fi credentials in the Arduino firmware.

Keep real credentials out of public commits.

---

## 9. Deploy the web files

The project requires:

```text
web hosting with PHP support
```

Upload the contents of:

```text
web/
```

to the server.

Configure the Arduino source with the corresponding server location.

The server and Arduino communicate through the generated JSON file.

---

## 10. Test the server

Verify that the JSON endpoint can be reached from a browser.

The response should follow the expected structure:

```json
{
  "mode": "r",
  "line1": "Hi there",
  "line2": "how are you",
  "line3": "ok"
}
```

---

## 11. Upload the Arduino firmware

Compile the sketch from:

```text
arduino/
```

and upload it to the UNO R4 WiFi.

Open Serial Monitor to inspect card operations during initial testing.

---

# ✍️ Writing a poem

Use the web interface to enter three lines.

For example:

```text
line 1:
Silent antenna

line 2:
memory waits

line 3:
signal fades
```

Every line must fit into:

```text
16 characters
```

Switch to:

```text
WRITE
```

mode and place a blank MIFARE Classic card on the reader.

The device writes the three lines and initializes the reading counter.

---

# 📖 Reading a poem

Switch the web interface to:

```text
READ
```

mode.

Place the card near the reader.

The Arduino:

```text
1. detects the card
2. authenticates the required sectors
3. reads the poem
4. displays the three lines
5. updates the counter
```

After the third permitted reading, the poem is removed.

---

# 🔑 Using other card blocks

The reference layout assumes:

```text
Blocks 4, 5, 6 → free
Block 8        → free
```

If those locations are already occupied on a card you control, the source can be modified to use different **data blocks**.

Do not write poetry into a:

```text
Sector Trailer
```

unless you intentionally know how MIFARE access bits and keys work.

Changing trailer data incorrectly can make sectors inaccessible.

---

# 🖨️ 3D-printed enclosure

The enclosure was designed in **Fusion 360**.

It consists of two STL files:

```text
Cuerpo.stl
Tapa.stl
```

Printing information:

```text
Material: PLA
Supports: No
```

The model is available at:

**[MIFARE Poetry NFC Enclosure — Cults3D](https://cults3d.com/en/3d-model/art/mifare-poetry-nfc-enclosure)**

The enclosure is designed around the:

- Arduino
- RFID reader
- OLED
- NFC-card interaction area

---

# 🎥 Demo

## MIFARE Poetry with Arduino UNO R4 WiFi and RC522

**[▶️ Watch on YouTube](https://www.youtube.com/watch?v=C4U-hpYl5XQ)**

TikTok demo:

**[▶️ Watch on TikTok](https://www.tiktok.com/@ronibandini/video/7407427604963167494)**

---

# 🧠 Why NFC poetry?

The project takes a technology normally associated with:

```text
transportation
access
payments
identification
inventory
```

and gives it a different function:

```text
literature
```

A card that normally says:

```text
Who are you?
```

or:

```text
How much credit remains?
```

instead says:

```text
three short lines
```

for three readings.

The storage limitations are not treated as a problem to overcome.

They are the specification of the literary form.

---

# 🔬 Ideas for extending the project

1. **🪪 Card-to-card poetry exchange** — allow two devices to copy, trade, or mutate poems while preserving the three-reading rule.

2. **🖨️ Final-reading receipt** — print the poem on thermal paper immediately before deleting its NFC version.

3. **🤖 Local LLM generator** — generate three-line poems under the strict 16-character-per-line constraint, then validate them before writing the card.

---

# 📰 External references

## 🗞️ Independent editorial coverage

### Arduino Blog — Storing ephemeral micropoetry on RFID cards for bite-sized readings

The Arduino team published a dedicated feature about MIFARE Poetry on **August 26, 2024**.

The article covers:

- the three-line poetic format
- 16-character line limit
- MIFARE Classic storage
- Arduino UNO R4 WiFi
- MFRC522
- OLED display
- three-reading lifecycle
- automatic deletion

**[Read the Arduino article](https://blog.arduino.cc/2024/08/26/storing-ephemeral-micropoetry-on-rfid-cards-for-bite-sized-readings/)**

---

## 📰 PlanetArduino

PlanetArduino also highlighted the project as an Arduino community build, describing it as an unusual reuse of MIFARE NFC cards for ephemeral micropoetry.

**[MIFARE Poetry — PlanetArduino](https://www.planetarduino.org/?cat=11060)**

---

# 🛠️ Project tutorial

## Hackster.io

### MIFARE Poetry with Arduino UNO R4 WiFi

The complete build tutorial was published on August 25, 2024.

It documents:

- MIFARE Classic memory organization
- the three poetry rules
- Arduino UNO R4 WiFi
- MFRC522 wiring
- OLED wiring
- card blocks
- read counter
- PHP interface
- JSON communication
- write mode
- read mode
- automatic Python poetry generation
- enclosure
- demo

**[MIFARE Poetry with Arduino UNO R4 WiFi — Hackster.io](https://www.hackster.io/roni-bandini/mifare-poetry-with-arduino-uno-r4-wifi-1358b7)**

---

# ✍️ Medium

## Poesía MIFARE

The Spanish-language project article explains the concept, memory architecture, reading rules, web interface, automatic generator, rhyming version, and physical device.

**[Poesía MIFARE — Medium](https://bandini.medium.com/poes%C3%ADa-mifare-fd46dd8a8658)**

---

# 📕 Contracultura Maker

MIFARE Poetry is part of a broader collection of projects that combine literature, obsolete or everyday technologies, electronics, programming, and unconventional interfaces.

More projects and context are collected in:

**[Contracultura Maker — book](https://bandini.medium.com/libro-de-contracultura-maker-94d1bb0d951c)**

---

# 📚 Useful references

- **[Arduino UNO R4 WiFi](https://docs.arduino.cc/hardware/uno-r4-wifi/)**
- **[MIFARE Classic EV1 1K — NXP](https://www.nxp.com/products/rfid-nfc/mifare-hf/mifare-classic/mifare-classic-ev1-1k-4k%3AMF1S50YYX_V1)**
- **[MIFARE Classic EV1 1K datasheet](https://www.nxp.com/docs/en/data-sheet/MF1S50YYX_V1.pdf)**
- **[MFRC522 datasheet](https://www.nxp.com/docs/en/data-sheet/MFRC522.pdf)**
- **[MFRC522 Arduino library](https://github.com/miguelbalboa/rfid)**
- **[Rhyming Dictionary](https://rhyme.sourceforge.net/)**
- **[MIFARE Poetry enclosure](https://cults3d.com/en/3d-model/art/mifare-poetry-nfc-enclosure)**

---

# 🔗 You may also be interested in...

Other projects by **Roni Bandini** combining literature, constrained interfaces, physical media, and unusual machines.

## 🧠📖 AI Remember

**A local-LLM generative literature system inspired by Joe Brainard and Georges Perec, producing fictional memories beginning with “Me acuerdo…”.**

Like MIFARE Poetry, it defines a formal literary constraint and makes software generate text inside it.

**[github.com/ronibandini/AIRemember](https://github.com/ronibandini/AIRemember)**

---

## 📚⌨️ Kindle Typewriter

**A jailbroken Kindle Paperwhite transformed into a distraction-free typewriter connected to a Raspberry Pi and thermal printer.**

Another project where an existing technological object becomes a new literary interface.

**[github.com/ronibandini/Kindle-Typewriter](https://github.com/ronibandini/Kindle-Typewriter)**

---

## 🌱🔊 The Sound Machine

**An Arduino literary machine inspired by Roald Dahl, converting soil-moisture measurements into prerecorded plant voices.**

Another experiment that turns literature into a physical electronic device.

**[github.com/ronibandini/TheSoundMachine](https://github.com/ronibandini/TheSoundMachine)**

---

# ⚠️ Notes

## ASCII and character length

The 16-character rule maps cleanly to a 16-byte block when using simple single-byte characters.

UTF-8 characters such as:

```text
á
é
ñ
ø
漢
🙂
```

may require more than one byte.

If using arbitrary Unicode text, validate:

```text
byte length
```

rather than only the number of displayed characters.

---

## Sector trailers

Never treat blocks:

```text
3
7
11
15
...
```

as ordinary data blocks on MIFARE Classic 1K.

They contain sector keys and access conditions.

Writing invalid access data can lock you out of the corresponding sector.

---

# 📜 License

MIFARE Poetry is released under the **MIT License**.

See [`LICENSE`](LICENSE) for details.

---

# 👤 Author

**Roni Bandini**

Maker, AI developer, electronic artist and writer.

- 🐙 GitHub: [@ronibandini](https://github.com/ronibandini)
- 💼 LinkedIn: [Roni Bandini](https://www.linkedin.com/in/ronibandini/)
- 📸 Instagram: [@ronibandini](https://www.instagram.com/ronibandini/)
- 🐦 X: [@RoniBandini](https://x.com/RoniBandini)
- ✍️ Medium: [bandini.medium.com](https://bandini.medium.com/)
- 🛠️ Hackster: [Roni Bandini](https://www.hackster.io/roni-bandini)
- 🔧 Hackaday.io: [Roni Bandini](https://hackaday.io/ronibandini)

Buenos Aires, Argentina.
