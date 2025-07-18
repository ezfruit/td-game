#include "wave.h"

std::vector<GameWave> waveDefinitions = {
    // Wave 1
    {
        {
            { "Slime", 0.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
        }
    },
    
    // Wave 2
    {
        {
            { "Slime", 0.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
        }
    },

    // Wave 3
    {
        {
            { "Slime", 0.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Slime", 0.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
        }
    },

    // Wave 4
    {
        {
            { "Slime", 0.0f },
            { "Slime", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
        }
    },

    // Wave 5
    {
        {
            { "Knight", 0.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Brute", 1.0f},
        }
    },

    // Wave 6
    {
        {
            { "Slime", 0.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
        }
    },

    // Wave 7
    {
        {
            { "Brute", 0.0f},
            { "Brute", 3.0f},
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Slime", 0.5f },
            { "Slime", 0.5f },
            { "Slime", 0.5f },
            { "Slime", 0.5f },
            { "Slime", 0.5f },
            { "Fire Imp", 0.5f },
            { "Fire Imp", 0.5f },
        }
    },

    // Wave 8
    {
        {
            { "Slime", 0.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Brute", 1.0f },
            { "Brute", 3.0f },
            { "Spider Queen", 3.0f },
        }
    },

    // Wave 9
    {
        {
            { "Fire Imp", 0.0f },
            { "Fire Imp", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Slime", 1.0f },
            { "Brute", 1.0f },
            { "Brute", 3.0f },
            { "Brute", 3.0f },
            { "Brute", 3.0f },
            { "Brute", 3.0f },
        }
    },

    // Wave 10
    {
        {
            { "Brute", 1.0f },
            { "Brute", 3.0f },
            { "Brute", 3.0f },
            { "Brute", 3.0f },
            { "Brute", 3.0f },
            { "Brute", 3.0f },
            { "Arcane Shell", 1.0f },
            { "Arcane Shell", 1.0f },
            { "Arcane Shell", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
        }
    },

    // Wave 11
    {
        {
            { "Fire Imp", 0.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
        }
    },

    // Wave 12
    {
        {
            { "Knight", 0.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Knight", 1.0f },
            { "Brute", 1.0f },
            { "Brute", 3.0f },
            { "Brute", 3.0f },
            { "Brute", 3.0f },
            { "Brute", 3.0f },
            { "Brute", 3.0f },
            { "Brute", 3.0f },
            { "Brute", 3.0f },
            { "Brute", 3.0f },
            { "Brute", 3.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Arcane Shell", 1.0f },
            { "Arcane Shell", 1.0f },
            { "Flux", 1.0f },
            { "Flux", 1.0f },
            { "Flux", 1.0f },
            { "Spider Queen", 1.0f },
            { "Spider Queen", 3.0f },
        }
    },

    // Wave 13
    {
        {
            { "Fire Imp", 0.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Arcane Shell", 1.0f },
            { "Arcane Shell", 1.0f },
            { "Arcane Shell", 1.0f },
            { "Brute", 1.0f },
            { "Brute", 2.0f },
            { "Flux", 1.0f },
            { "Flux", 1.0f },
            { "Husk", 1.0f },
            { "Husk", 1.0f },
            { "Husk", 1.0f },
            { "Flux", 1.0f },
            { "Flux", 1.0f },
            { "Brute", 1.0f },
            { "Brute", 2.0f },
            { "Arcane Shell", 1.0f },
            { "Arcane Shell", 1.0f },
            { "Arcane Shell", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 1.0f },
        }
    },

    // Wave 14
    {
        {
            { "Brute", 0.0f },
            { "Brute", 0.5f },
            { "Brute", 0.5f },
            { "Brute", 0.5f },
            { "Brute", 0.5f },
            { "Brute", 0.5f },
            { "Brute", 0.5f },
            { "Brute", 0.5f },
            { "Brute", 0.5f },
            { "Brute", 0.5f },
            { "Flux", 1.0f },
            { "Flux", 2.0f },
            { "Flux", 2.0f },
            { "Husk", 2.0f },
            { "Husk", 2.0f },
            { "Husk", 2.0f },
            { "Arcane Shell", 1.0f },
            { "Arcane Shell", 1.0f },
            { "Exoskeleton", 1.0f },
            { "Spider Queen", 1.0f },
            { "Spider Queen", 1.0f },
        }
    },

    // Wave 15
    {
        {
            { "Husk", 0.0f },
            { "Husk", 1.0f },
            { "Husk", 1.0f },
            { "Exoskeleton", 1.0f },
            { "Fire Imp", 0.5f },
            { "Exoskeleton", 4.0f },
            { "Fire Imp", 0.5f },
            { "Goliath", 3.0f },
            { "Knight", 1.0f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Knight", 0.5f },
            { "Arcane Shell", 1.0f },
            { "Arcane Shell", 1.0f },
            { "Arcane Shell", 1.0f },
            { "Arcane Shell", 1.0f },
            { "Arcane Shell", 1.0f },
        }
    },

    // Wave 16
    {
        {
            { "Flux", 0.0f },
            { "Flux", 2.0f },
            { "Flux", 2.0f },
            { "Flux", 2.0f },
            { "Flux", 2.0f },
            { "Sludge", 1.0f },
            { "Sludge", 2.0f },
            { "Sludge", 2.0f },
            { "Exoskeleton", 5.0f },
            { "Exoskeleton", 5.0f },
            { "Exoskeleton", 5.0f },
            { "Arcane Shell", 1.0f },
            { "Husk", 2.0f },
            { "Arcane Shell", 1.0f },
            { "Husk", 2.0f },
            { "Arcane Shell", 1.0f },
            { "Husk", 2.0f },
            { "Arcane Shell", 1.0f },
            { "Husk", 2.0f },
            { "Arcane Shell", 1.0f },
            { "Husk", 2.0f },
            { "Spider Queen", 1.0f },
            { "Spider Queen", 1.0f },
            { "Spider Queen", 1.0f },
        }
    },

    // Wave 17
    {
        {
            { "Arcane Shell", 0.0f },
            { "Arcane Shell", 0.5f },
            { "Arcane Shell", 0.5f },
            { "Arcane Shell", 0.5f },
            { "Arcane Shell", 0.5f },
            { "Sludge", 1.0f },
            { "Sludge", 1.0f },
            { "Exoskeleton", 1.0f },
            { "Husk", 1.0f },
            { "Flux", 1.0f },
            { "Flux", 1.0f },
            { "Sludge", 1.0f },
            { "Sludge", 1.0f },
            { "Exoskeleton", 1.0f },
            { "Husk", 1.0f },
            { "Lava Golem", 1.0f },
            { "Fire Imp", 0.5f },
            { "Lava Golem", 1.0f },
            { "Fire Imp", 0.5f },
            { "Lava Golem", 1.0f },
            { "Fire Imp", 0.5f },
        }
    },

    // Wave 18
    {
        {
            { "Slime", 0.0f },
            { "Slime", 0.25f },
            { "Slime", 0.25f },
            { "Slime", 0.25f },
            { "Slime", 0.25f },
            { "Slime", 0.25f },
            { "Slime", 0.25f },
            { "Slime", 0.25f },
            { "Slime", 0.25f },
            { "Slime", 0.25f },
            { "Knight", 1.0f },
            { "Knight", 0.25f },
            { "Knight", 0.25f },
            { "Knight", 0.25f },
            { "Knight", 0.25f },
            { "Knight", 0.25f },
            { "Knight", 0.25f },
            { "Knight", 0.25f },
            { "Knight", 0.25f },
            { "Knight", 0.25f },
            { "Fire Imp", 1.0f },
            { "Fire Imp", 0.5f },
            { "Fire Imp", 0.5f },
            { "Fire Imp", 0.5f },
            { "Fire Imp", 0.5f },
            { "Fire Imp", 0.5f },
            { "Fire Imp", 0.5f },
            { "Fire Imp", 0.5f },
            { "Brute", 1.0f},
            { "Brute", 0.5f},
            { "Brute", 0.5f},
            { "Brute", 0.5f},
            { "Brute", 0.5f},
            { "Brute", 0.5f},
            { "Brute", 0.5f},
            { "Brute", 0.5f},
            { "Arcane Shell", 1.0f },
            { "Arcane Shell", 0.5f },
            { "Arcane Shell", 0.5f },
            { "Arcane Shell", 0.5f },
            { "Arcane Shell", 0.5f },
            { "Arcane Shell", 0.5f },
            { "Flux", 1.0f },
            { "Flux", 0.5f },
            { "Flux", 0.5f },
            { "Flux", 0.5f },
            { "Flux", 0.5f },
            { "Flux", 0.5f },
            { "Spider Queen", 1.0f },
            { "Spider Queen", 2.0f },
            { "Spider Queen", 2.0f },
            { "Spider Queen", 2.0f },
            { "Spider Queen", 2.0f },
            { "Spider Queen", 2.0f },
            { "Husk", 1.0f },
            { "Husk", 1.0f },
            { "Husk", 1.0f },
            { "Husk", 1.0f },
            { "Husk", 1.0f },
            { "Husk", 1.0f },
            { "Sludge", 2.0f },
            { "Sludge", 2.0f },
            { "Sludge", 2.0f },
            { "Sludge", 2.0f },
            { "Exoskeleton", 3.0f },
            { "Exoskeleton", 3.0f },
            { "Exoskeleton", 3.0f },
            { "Lava Golem", 4.0f },
            { "Lava Golem", 4.0f },
            { "Goliath", 5.0f },
        }
    },

    // Wave 19
    {
        {
            { "Flux", 0.0f },
            { "Husk", 0.25f },
            { "Flux", 0.5f },
            { "Husk", 0.25f },
            { "Flux", 0.5f },
            { "Husk", 0.25f },
            { "Flux", 0.5f },
            { "Husk", 0.25f },
            { "Flux", 0.5f },
            { "Husk", 0.25f },
            { "Flux", 0.5f },
            { "Arcane Shell", 0.5f },
            { "Arcane Shell", 0.5f },
            { "Sludge", 0.5f },
            { "Arcane Shell", 0.5f },
            { "Arcane Shell", 0.5f },
            { "Sludge", 0.5f },
            { "Arcane Shell", 0.5f },
            { "Arcane Shell", 0.5f },
            { "Sludge", 0.5f },
            { "Exoskeleton", 1.0f },
            { "Exoskeleton", 1.0f },
            { "Exoskeleton", 1.0f },
            { "Goliath", 2.0f },
            { "Goliath", 2.0f },
            { "Obsidian Behemoth", 3.0f},
            { "Husk", 5.0f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
        }
    },

    // Wave 20
    {
        {
            { "Exoskeleton", 0.0f },
            { "Husk", 0.25f },
            { "Exoskeleton", 0.25f },
            { "Husk", 0.25f },
            { "Exoskeleton", 0.25f },
            { "Husk", 0.25f },
            { "Exoskeleton", 0.25f },
            { "Husk", 0.25f },
            { "Exoskeleton", 0.25f },
            { "Husk", 0.25f },
            { "Sludge", 0.25f },
            { "Husk", 0.25f },
            { "Flux", 0.25f },
            { "Husk", 0.25f },
            { "Sludge", 0.25f },
            { "Husk", 0.25f },
            { "Flux", 0.25f },
            { "Husk", 0.25f },
            { "Sludge", 0.25f },
            { "Husk", 0.25f },
            { "Flux", 0.25f },
            { "Husk", 0.25f },
            { "Sludge", 0.25f },
            { "Husk", 0.25f },
            { "Flux", 0.25f },
            { "Husk", 0.25f },
            { "Sludge", 0.25f },
            { "Husk", 0.25f },
            { "Sludge", 0.25f },
            { "Husk", 0.25f },
            { "Sludge", 0.25f },
            { "Husk", 0.25f },
            { "Sludge", 0.25f },
            { "Husk", 0.25f },
            { "Spider Queen", 0.25f },
            { "Husk", 0.25f },
            { "Spider Queen", 0.25f },
            { "Husk", 0.25f },
            { "Spider Queen", 0.25f },
            { "Husk", 0.25f },
            { "Lava Golem", 1.0f },
            { "Husk", 0.25f },
            { "Lava Golem", 1.0f },
            { "Husk", 0.25f },
            { "Lava Golem", 1.0f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Goliath", 1.5f },
            { "Husk", 0.25f },
            { "Goliath", 1.5f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Obsidian Behemoth", 2.0f},
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
        }
    },

    // Wave 21
    {
        {
            { "Sludge", 0.0f },
            { "Sludge", 0.5f },
            { "Sludge", 0.5f },
            { "Sludge", 0.5f },
            { "Sludge", 0.5f },
            { "Sludge", 0.5f },
            { "Sludge", 0.5f },
            { "Sludge", 0.5f },
            { "Sludge", 0.5f },
            { "Sludge", 0.5f },
            { "Sludge", 0.5f },
            { "Sludge", 0.5f },
            { "Sludge", 0.5f },
            { "Sludge", 0.5f },
            { "Sludge", 0.5f },
            { "Sludge", 0.5f },
            { "Sludge", 0.5f },
            { "Sludge", 0.5f },
            { "Sludge", 0.5f },
            { "Sludge", 0.5f },
        }
    },

    // Wave 22
    {
        {
            { "Lava Golem", 0.0f },
            { "Lava Golem", 1.0f },
            { "Lava Golem", 1.0f },
            { "Flux", 0.5f },
            { "Flux", 0.5f },
            { "Flux", 0.5f },
            { "Flux", 0.5f },
            { "Flux", 0.5f },
            { "Flux", 0.5f },
            { "Flux", 0.5f },
            { "Flux", 0.5f },
            { "Flux", 0.5f },
            { "Flux", 0.5f },
            { "Exoskeleton", 1.0f },
            { "Exoskeleton", 1.0f },
            { "Exoskeleton", 1.0f },
            { "Exoskeleton", 1.0f },
            { "Exoskeleton", 1.0f },
            { "Spider Queen", 0.5f },
            { "Spider Queen", 0.5f },
            { "Ravager", 1.0f },
            { "Ravager", 1.0f },
        }
    },

    // Wave 23
    {
        {
            { "Sludge", 0.0f },
            { "Sludge", 0.25f },
            { "Sludge", 0.25f },
            { "Sludge", 0.25f },
            { "Sludge", 0.25f },
            { "Arcane Shell", 0.25f },
            { "Arcane Shell", 0.25f },
            { "Arcane Shell", 0.25f },
            { "Arcane Shell", 0.25f },
            { "Arcane Shell", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Goliath", 0.5f },
            { "Goliath", 0.5f },
            { "Goliath", 0.5f },
            { "Goliath", 0.5f },
            { "Goliath", 0.5f },
            { "Obsidian Behemoth", 1.0f},
            { "Obsidian Behemoth", 1.0f},
        }
    },

    // Wave 24
    {
        {
            { "Obsidian Behemoth", 0.0f},
            { "Sludge", 0.25f },
            { "Sludge", 0.25f },
            { "Sludge", 0.25f },
            { "Sludge", 0.25f },
            { "Sludge", 0.25f },
            { "Ravager", 0.5f },
            { "Flux", 0.25f },
            { "Flux", 0.25f },
            { "Flux", 0.25f },
            { "Flux", 0.25f },
            { "Flux", 0.25f },
            { "Husk", 0.5f },
            { "Husk", 0.5f },
            { "Husk", 0.5f },
            { "Husk", 0.5f },
            { "Husk", 0.5f },
            { "Ravager", 0.5f },
            { "Spider Queen", 0.25f },
            { "Spider Queen", 0.25f },
            { "Spider Queen", 0.25f },
            { "Spider Queen", 0.25f },
            { "Arcane Warden", 1.0f},
        }
    },

    // Wave 25
    {
        {
            { "Goliath", 0.0f },
            { "Husk", 1.0f },
            { "Husk", 0.5f },
            { "Husk", 0.5f },
            { "Husk", 0.5f },
            { "Husk", 0.5f },
            { "Husk", 0.5f },
            { "Husk", 0.5f },
            { "Husk", 0.5f },
            { "Husk", 0.5f },
            { "Husk", 0.5f },
            { "Goliath", 0.5f },
            { "Ravager", 1.0f },
            { "Ravager", 1.0f },
            { "Ravager", 1.0f },
            { "Ravager", 1.0f },
            { "Ravager", 1.0f },
            { "Ravager", 1.0f },
            { "Ravager", 1.0f },
            { "Ravager", 1.0f },
            { "Arcane Warden", 1.0f},
            { "Arcane Warden", 1.0f},
        }
    },

    // Wave 26
    {
        {
            { "Exoskeleton", 0.0f },
            { "Sludge", 0.1f },
            { "Exoskeleton", 0.5f },
            { "Sludge", 0.1f },
            { "Exoskeleton", 0.5f },
            { "Sludge", 0.1f },
            { "Exoskeleton", 0.5f },
            { "Sludge", 0.1f },
            { "Exoskeleton", 0.5f },
            { "Sludge", 0.1f },
            { "Exoskeleton", 0.5f },
            { "Sludge", 0.1f },
            { "Exoskeleton", 0.5f },
            { "Sludge", 0.1f },
            { "Exoskeleton", 0.5f },
            { "Sludge", 0.1f },
            { "Exoskeleton", 0.5f },
            { "Sludge", 0.1f },
            { "Exoskeleton", 0.5f },
            { "Sludge", 0.1f },
            { "Arcane Warden", 0.5f},
            { "Ravager", 0.5f },
            { "Ravager", 0.5f },
            { "Ravager", 0.5f },
            { "Ravager", 0.5f },
            { "Arcane Warden", 0.5f},
            { "Obsidian Behemoth", 0.5f},
            { "Ravager", 1.0f },
            { "Ravager", 1.0f },
            { "Ravager", 1.0f },
            { "Husk", 0.5f },
            { "Husk", 0.5f },
            { "Husk", 0.5f },
            { "Husk", 0.5f },
            { "Husk", 0.5f },
        }
    },

    // Wave 27
    {
        {
            { "Goliath", 0.0f },
            { "Flux", 0.5f },
            { "Flux", 0.5f },
            { "Flux", 0.5f },
            { "Flux", 0.5f },
            { "Flux", 0.5f },
            { "Spider Queen", 0.5f},
            { "Spider Queen", 0.5f},
            { "Spider Queen", 0.5f},
            { "Spider Queen", 0.5f},
            { "Spider Queen", 0.5f},
            { "Arcane Warden", 0.5f},
            { "Arcane Warden", 0.5f},
            { "Arcane Warden", 0.5f},
            { "Arcane Warden", 0.5f},
            { "Arcane Warden", 0.5f},
        }
    },

    // Wave 28
    {
        {
            { "Flux", 0.0f },
            { "Arcane Shell", 0.1f },
            { "Flux", 0.1f },
            { "Arcane Shell", 0.1f },
            { "Flux", 0.1f },
            { "Arcane Shell", 0.1f },
            { "Flux", 0.1f },
            { "Arcane Shell", 0.1f },
            { "Flux", 0.1f },
            { "Lava Golem", 0.5f },
            { "Lava Golem", 0.5f },
            { "Sludge", 1.0f },
            { "Sludge", 1.0f },
            { "Sludge", 1.0f },
            { "Sludge", 1.0f },
            { "Sludge", 1.0f },
            { "Ravager", 0.5f },
            { "Ravager", 0.5f },
            { "Arcane Warden", 0.5f},
            { "Obsidian Behemoth", 0.5f},
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
            { "Husk", 0.25f },
        }
    },

    // Wave 29
    {
        {
            { "Lava Golem", 0.0f },
            { "Lava Golem", 0.25f },
            { "Lava Golem", 0.25f },
            { "Lava Golem", 0.25f },
            { "Lava Golem", 0.25f },
            { "Ravager", 0.5f },
            { "Ravager", 0.5f },
            { "Ravager", 0.5f },
            { "Ravager", 0.5f },
            { "Ravager", 0.5f },
            { "Arcane Warden", 0.25f},
            { "Exoskeleton", 0.25f },
            { "Sludge", 0.25f },
            { "Exoskeleton", 0.25f },
            { "Sludge", 0.25f },
            { "Exoskeleton", 0.25f },
            { "Sludge", 0.25f },
            { "Exoskeleton", 0.25f },
            { "Sludge", 0.25f },
            { "Exoskeleton", 0.25f },
            { "Sludge", 0.25f },
            { "Exoskeleton", 0.25f },
            { "Sludge", 0.25f },
            { "Spider Queen", 0.25f},
            { "Spider Queen", 0.25f},
            { "Arcane Warden", 0.5f},
            { "Goliath", 0.25f },
            { "Goliath", 0.25f },
            { "Goliath", 0.25f },
            { "Obsidian Behemoth", 0.25f},
            { "Obsidian Behemoth", 0.25f},
        }
    },

    // Wave 30
    {
        {
            { "Sludge", 0.0f },
            { "Flux", 0.25f },
            { "Sludge", 0.25f },
            { "Flux", 0.25f },
            { "Sludge", 0.25f },
            { "Flux", 0.25f },
            { "Sludge", 0.25f },
            { "Flux", 0.25f },
            { "Spider Queen", 0.25f},
            { "Spider Queen", 0.25f},
            { "Ravager", 0.5f },
            { "Husk", 0.25f },
            { "Ravager", 0.5f },
            { "Husk", 0.25f },
            { "Ravager", 0.5f },
            { "Husk", 0.25f },
            { "Ravager", 0.5f },
            { "Husk", 0.25f },
            { "Ravager", 0.5f },
            { "Husk", 0.25f },
            { "Ravager", 0.5f },
            { "Husk", 0.25f },
            { "Arcane Warden", 0.5f},
            { "Arcane Warden", 0.5f},
            { "Goliath", 0.5f },
            { "Goliath", 0.5f },
            { "Obsidian Behemoth", 0.25f},
            { "Null Imp", 1.0f},
        }
    },

    // Wave 31
    {
        {
            { "Ravager", 0.0f },
            { "Ravager", 0.5f },
            { "Ravager", 0.5f },
            { "Ravager", 0.5f },
            { "Ravager", 0.5f },
            { "Ravager", 0.5f },
            { "Ravager", 0.5f },
            { "Ravager", 0.5f },
            { "Ravager", 0.5f },
            { "Ravager", 0.5f },
            { "Null Imp", 0.5f},
            { "Husk", 0.5f },
            { "Exoskeleton", 0.0f },
            { "Husk", 0.5f },
            { "Exoskeleton", 0.0f },
            { "Arcane Warden", 0.5f},
            { "Sludge", 0.0f },
            { "Arcane Warden", 0.5f},
            { "Sludge", 0.0f },
            { "Lava Golem", 0.5f },
            { "Lava Golem", 0.5f },
            { "Goliath", 0.5f },
            { "Null Imp", 1.0f},
            { "Husk", 0.5f },
            { "Husk", 0.5f },
            { "Husk", 0.5f },
            { "Spider Queen", 0.25f},
            { "Spider Queen", 0.25f}
        }
    },

    // Wave 32
    {
        {
            { "Null Imp", 0.0f},
            { "Null Imp", 1.0f},
            { "Null Imp", 1.0f},
            { "Null Imp", 1.0f},
            { "Null Imp", 1.0f},
        }
    },

    // Wave 33
    {
        {
            { "Sludge", 0.0f },
            { "Sludge", 0.5f },
            { "Sludge", 0.5f },
            { "Sludge", 0.5f },
            { "Sludge", 0.5f },
            { "Arcane Shell", 0.5f},
            { "Arcane Shell", 0.5f},
            { "Exoskeleton", 0.5f },
            { "Exoskeleton", 0.5f },
            { "Exoskeleton", 0.5f },
            { "Ravager", 0.5f },
            { "Ravager", 0.5f },
            { "Ravager", 0.5f },
            { "Ravager", 0.5f },
            { "Ravager", 0.5f },
            { "Ravager", 0.5f },
            { "Goliath", 0.5f },
            { "Husk", 0.25f },
            { "Goliath", 0.5f },
            { "Husk", 0.25f },
            { "Goliath", 0.5f },
            { "Husk", 0.25f },
            { "Goliath", 0.5f },
            { "Husk", 0.25f },
            { "Goliath", 0.5f },
            { "Husk", 0.25f },
            { "Obsidian Behemoth", 0.5f},
            { "Husk", 0.25f },
            { "Obsidian Behemoth", 0.5f},
            { "Husk", 0.25f },
            { "Obsidian Behemoth", 0.5f},
            { "Husk", 0.25f },
            { "Arcane Warden", 0.5f},
            { "Husk", 0.25f },
            { "Arcane Warden", 0.5f},
            { "Husk", 0.25f },
            { "Arcane Warden", 0.5f},
            { "Husk", 0.25f },
            { "Lava Golem", 0.25f },
            { "Lava Golem", 0.25f },
            { "Lava Golem", 0.25f },
            { "The Host", 1.0f},
        }
    },

    // Wave 34
    {
        {
            { "Null Imp", 0.0f},
            { "Null Imp", 1.0f},
            { "Null Imp", 1.0f},
            { "Null Imp", 1.0f},
            { "Null Imp", 1.0f},
            { "Null Imp", 1.0f},
            { "Null Imp", 1.0f},
            { "Null Imp", 1.0f},
            { "Null Imp", 1.0f},
            { "Null Imp", 1.0f},
            { "The Host", 1.0f},
            { "Arcane Warden", 0.5f},
            { "Arcane Warden", 0.5f},
            { "Arcane Warden", 0.5f},
        }
    },

    // Wave 35
    {
        {
            { "Null Imp", 0.0f},
            { "Bulwark", 0.5f},
            { "Null Imp", 1.0f},
            { "Null Imp", 1.0f},
            { "Null Imp", 1.0f},
            { "Null Imp", 1.0f},
            { "Null Imp", 1.0f},
            { "Bulwark", 0.5f},
            { "Null Imp", 1.0f},
            { "Null Imp", 1.0f},
            { "The Host", 1.0f},
            { "Bulwark", 0.5f},
            { "Null Imp", 1.0f},
            { "Null Imp", 1.0f},
            { "Null Imp", 1.0f},
            { "Null Imp", 1.0f},
            { "Bulwark", 0.5f},
            { "The Host", 1.0f},
        }
    },

    // Wave 36
    {
        {
            { "Null Imp", 0.0f},
            { "Null Imp", 1.0f},
            { "Null Imp", 1.0f},
            { "Prime Sludge", 1.0f },
            { "Null Imp", 1.0f},
            { "Prime Sludge", 1.0f },
            { "Null Imp", 1.0f},
            { "Prime Sludge", 1.0f },
            { "Null Imp", 1.0f},
            { "Prime Sludge", 1.0f },
            { "Null Imp", 1.0f},
            { "Prime Sludge", 1.0f },
            { "Null Imp", 1.0f},
            { "Bulwark", 1.0f},
            { "Bulwark", 1.0f},
            { "Bulwark", 1.0f},
            { "Bulwark", 1.0f},
            { "Bulwark", 1.0f},
        }
    },

    // Wave 37
    {
        {
            { "Null Imp", 0.0f},
            { "Null Imp", 0.75f},
            { "Null Imp", 0.75f},
            { "Null Imp", 0.75f},
            { "Null Imp", 0.75f},
            { "Prime Sludge", 0.5f },
            { "Prime Sludge", 0.5f },
            { "Prime Sludge", 0.5f },
            { "Prime Sludge", 0.5f },
            { "Prime Sludge", 0.5f },
            { "Bulwark", 1.0f},
            { "Bulwark", 0.5f},
            { "Bulwark", 0.5f},
            { "Bulwark", 0.5f},
            { "Bulwark", 0.5f},
            { "Bulwark", 0.5f},
            { "Bulwark", 0.5f},
            { "Bulwark", 0.5f},
            { "The Host", 1.0f},
            { "The Host", 1.0f},
        }
    },

    // Wave 38
    {
        {
            { "Bulwark", 0.0f},
            { "Prime Sludge", 0.25f },
            { "Prime Sludge", 0.25f },
            { "Prime Sludge", 0.25f },
            { "Prime Sludge", 0.25f },
            { "Prime Sludge", 0.25f },
            { "Bulwark", 0.5f},
            { "Null Imp", 0.5f},
            { "Null Imp", 0.5f},
            { "Null Imp", 0.5f},
            { "Null Imp", 0.5f},
            { "Bulwark", 0.5f},
            { "Bulwark", 0.5f},
            { "Prime Sludge", 0.25f },
            { "Prime Sludge", 0.25f },
            { "Prime Sludge", 0.25f },
            { "Prime Sludge", 0.25f },
            { "Prime Sludge", 0.25f },
            { "The Host", 0.5f},
            { "Big Slime", 1.0f },
        }
    },

    // Wave 39
    {
        {
            { "Slime", 0.0f },
            { "Knight", 1.0f },
            { "Fire Imp", 1.0f },
            { "Brute", 1.0f },
            { "Spider Queen", 1.0f },
            { "Arcane Shell", 1.0f},
            { "Flux", 1.0f },
            { "Husk", 1.0f },
            { "Exoskeleton", 1.0f },
            { "Goliath", 1.0f },
            { "Sludge", 1.0f },
            { "Lava Golem", 1.0f },
            { "Obsidian Behemoth", 1.0f },
            { "Ravager", 1.0f },
            { "Arcane Warden", 1.0f },
            { "Null Imp", 1.0f},
            { "Null Imp", 0.5f},
            { "Null Imp", 0.5f},
            { "The Host", 1.0f},
            { "The Host", 0.5f},
            { "The Host", 0.5f},
            { "Bulwark", 1.0f},
            { "Bulwark", 0.5f},
            { "Bulwark", 0.5f},
            { "Prime Sludge", 1.0f},
            { "Prime Sludge", 1.0f},
            { "Prime Sludge", 1.0f},
            { "Big Slime", 1.0f },
            { "Big Slime", 2.0f },
            { "Big Slime", 2.0f },
        }
    },

    // Wave 40
    {
        {
            { "Exoskeleton", 0.0f },
            { "Exoskeleton", 0.25f },
            { "Exoskeleton", 0.25f },
            { "Exoskeleton", 0.25f },
            { "Exoskeleton", 0.25f },
            { "Obsidian Behemoth", 0.25f },
            { "Ravager", 0.5f },
            { "Obsidian Behemoth", 0.25f },
            { "Ravager", 0.5f },
            { "Obsidian Behemoth", 0.25f },
            { "Ravager", 0.5f },
            { "Obsidian Behemoth", 0.25f },
            { "Ravager", 0.5f },
            { "Obsidian Behemoth", 0.25f },
            { "Ravager", 0.5f },
            { "Null Imp", 0.25f},
            { "Ravager", 0.5f },
            { "Null Imp", 0.25f},
            { "Ravager", 0.5f },
            { "Null Imp", 0.25f},
            { "Ravager", 0.5f },
            { "Null Imp", 0.25f},
            { "Ravager", 0.5f },
            { "Null Imp", 0.25f},
            { "Arcane Warden", 0.5f },
            { "Arcane Warden", 0.5f },
            { "Prime Sludge", 0.5f},
            { "Bulwark", 0.5f},
            { "Prime Sludge", 0.5f},
            { "Bulwark", 0.5f},
            { "Prime Sludge", 0.5f},
            { "Bulwark", 0.5f},
            { "Prime Sludge", 0.5f},
            { "Bulwark", 0.5f},
            { "Prime Sludge", 0.5f},
            { "Bulwark", 0.5f},
            { "The Host", 0.5f},
            { "The Host", 0.5f},
            { "The Host", 0.5f},
            { "Big Slime", 1.0f },
            { "Big Slime", 1.0f },
            { "Big Slime", 1.0f },
            { "Big Slime", 1.0f },
            { "Big Slime", 1.0f },
            { "Fractured King", 1.0f },
        }
    },

};

const size_t totalWaves = waveDefinitions.size();