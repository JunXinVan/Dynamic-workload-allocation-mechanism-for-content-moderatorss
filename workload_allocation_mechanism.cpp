#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

enum ContentType { TEXT, IMAGE, VIDEO, AUDIO };
const std::string ContentTypeString[] = { "Text", "Image", "Video", "Audio" };

struct Content {
    ContentType type;
    int size; // In KB
    Content(ContentType type, int size) : type(type), size(size) {}
};

struct Moderator {
    std::string name;
    int age;
    int yearsOfExperience;
    int weeklyWorkHours;
    float fatigueLevel; // Derived from weeklyWorkHours
    std::vector<ContentType> lastWeekTypes;

    Moderator(std::string name, int age, int yearsOfExperience, int weeklyWorkHours = 0, float fatigueLevel = 0.0)
        : name(name), age(age), yearsOfExperience(yearsOfExperience), weeklyWorkHours(weeklyWorkHours), fatigueLevel(fatigueLevel) {
        updateFatigueLevel(); // Ensure fatigue level is correctly set based on initial weeklyWorkHours
        lastWeekTypes = std::vector<ContentType>(); // Explicitly initializing, though not strictly necessary
    }

    void updateFatigueLevel() {
        // Example calculation, can be adjusted
        fatigueLevel = weeklyWorkHours / 40.0; // Assuming 40 hours is a full work week
    }

    void addWorkHours(int hours) {
        weeklyWorkHours += hours;
        updateFatigueLevel();
    }

    void resetWeek() {
        lastWeekTypes.clear();
        weeklyWorkHours = 0;
        fatigueLevel = 0.0;
    }
};

class WorkAllocator {
public:
    std::vector<Content> contents;
    std::vector<Moderator> moderators;

    void addContent(const Content& content) {
        contents.push_back(content);
    }

    void addModerator(const Moderator& moderator) {
        moderators.push_back(moderator);
    }

    void distributeWork() {
        // Simple distribution logic, can be enhanced
        for (auto& content : contents) {
            // Find the least fatigued moderator with the least recent history of this content type
            std::sort(moderators.begin(), moderators.end(), [&](const Moderator& a, const Moderator& b) {
                return a.fatigueLevel < b.fatigueLevel || (a.fatigueLevel == b.fatigueLevel && std::find(a.lastWeekTypes.begin(), a.lastWeekTypes.end(), content.type) == a.lastWeekTypes.end());
            });

            if (!moderators.empty()) {
                // Assign content to the chosen moderator
                Moderator& chosenOne = moderators.front();
                chosenOne.lastWeekTypes.push_back(content.type);
                chosenOne.addWorkHours(content.size / 10); // Example: 1 hour per 10KB
                std::cout << "Assigned " << ContentTypeString[content.type] << " content of size " << content.size << "KB to " << chosenOne.name << std::endl;
            }
        }

        // Reset for the next week
        for (auto& mod : moderators) {
            mod.resetWeek();
        }
    }
};

int main() {
    // Create Content
    Content textContent(TEXT, 100);    // 100KB Text Content
    Content imageContent(IMAGE, 200);  // 200KB Image Content
    Content videoContent(VIDEO, 300);  // 300KB Video Content

    // Create Moderators with initial values
    Moderator alice("Alice", 30, 5);
    Moderator bob("Bob", 25, 3, 20);   // 20 hours already worked this week
    Moderator charlie("Charlie", 28, 7, 10, 0.25); // 10 hours worked, custom fatigue level (though it will be recalculated)

    // Instantiate WorkAllocator and add content and moderators
    WorkAllocator allocator;
    allocator.addContent(textContent);
    allocator.addContent(imageContent);
    allocator.addContent(videoContent);
    allocator.addModerator(alice);
    allocator.addModerator(bob);
    allocator.addModerator(charlie);

    // Distribute work
    allocator.distributeWork();

    return 0;
}