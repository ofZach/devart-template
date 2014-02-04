# DevArt Template

This repository is where you will populate your Project Page on the DevArt site. It is also where you will build your code and document your process. Your page on DevArt will change every time you make changes to your code and push this repository to GitHub, pending moderation by the DevArt team. You will be able to preview the changes on your Project Page on DevArt after you push to GitHub. All submissions in GitHub will need to be entered in English. 


All updates are made using [Github Flavoured Markdown Language](https://help.github.com/articles/github-flavored-markdown). Please ensure that your files use the GitHub Flavoured formatting.

## Folder and File Structure

- **readme.md**
*Where to find the full instructions for how to use the DevArt template.*

- **project_summary.md** *Where to edit your project as you progress, from Cover Image, project description to final prototype.*

- **project_technologies.json** *Where to define technologies used, including at least one Google technology.*

- **project_posts/** *Where to add updates of your progress.*

- **project_images/** *Where to add and store images for your Project Summary and Project Posts.*

- **project_code/** *Where to keep your source code and stable releases of your project. Structure anything inside this folder as you see fit.*

## Instructions

### Your Cover Image  

To start with you’ll need to replace the file **cover.jpg** inside the **/project_images** folder with a image of your choice to add your Cover Image on your Project Page. This will make your project to appear in the DevArt Gallery. A **1000 x 500 pixels** jpeg is the optimum size.

This can be anything from your inspiration to your initial sketch, and can be edited as many times as you like during your progress. If no Cover Image is defined then the image from your most recent Project Post will be used and will automatically update with every new post.
**If no Cover Image or Project Post image is available then your project will not appear in the DevArt Gallery.**

### Your Project Summary

Edit **project_summary.md** to describe your project title, idea, vision and where ultimately the final results of your project are displayed. This too can be edited as many times as you like during your progress - from concept and early sketches to the finished piece. We encourage you to always update with the latest images, videos, and links of your project. This will be the first thing judges will look at when they go to your Project Page. The more complete, the better. 
**For any videos upload to YouTube and insert the YouTube embed link.**

### Your Project Technologies

Edit  **project_technologies.json**  using the example JSON structure to define the technologies you're using on your project within the following categories: ```Toolkits```, ```Platforms```& ```APIs```. The 4th category ```Languages``` is automatically defined through GitHub. You can also add themes as tags. Make sure your JSON conforms to the structure of the template and is validated. By creating a list of technologies and themes, your project will be searchable in the DevArt Gallery. See example below:

```
{
    "technologies": {
        "apis": [
            "Web Speech",
            "Google Plus"
        ],
        "platforms": [
            "Google Compute Engine"
        ],
        "toolkits": [
            "Node.js",
            "Three.js",
            "Mongodb"
        ]
    },
    "themes": [
        "Wishes",
        "Particles",
        "Dreams"
    ]
}
```

**Make sure you include at least one Google technology to be eligible for judging. See Judging Criteria on [Enter Now Page](https://devart.withgoogle.com/#/enter-now/judging-criteria) for full details on Google technologies.**

### Your Project Posts

To create a Project Post,  duplicate the example post in **project_posts/** folder renaming the file using the same convention **YYYY-MM-DD-POST-TITLE.MD** that applies to date and title of your post at the time.

This section is like a blog of your progress, where you will document your process—from early napkin sketches to working prototypes. As soon as you have a rough idea of your project, you should create your first Project Post. We encourage you to be as visual as possible including links, images and video.  The Commissioned Interactive Artists will be using the same template so be sure to follow their ‘open process’ as their projects unfold.  **Get started early and post often - you'll need at least 5 Posts to be eligible for judging. See what the judges will be looking for in the Judging Criteria on the [Enter Now Page](https://devart.withgoogle.com/#/enter-now/judging-criteria) of the website.**
**For any videos upload to YouTube and insert the YouTube embed link.**

### Mark Your Project as Complete 
When you have completed your submission, anytime before the deadline of **28 March 2014, 18:00 GMT**, it’s essential that you mark your project as complete on your Project Page. **Only projects that have been marked as ‘complete’ are eligible for judging.** Before you mark your project as as complete please double-check you’ve done the following:

1. Your **Project Summary** is fully complete including your project name, a detailed description of your work and supporting images and videos of your completed project. If possible, include a link to a working version of your project. And don’t forget to update your Cover Image with an image that best represents your final piece.
2. Your **Project Technologies** includes keywords for all themes explored and you have defined all technologies used for your project, including at least one Google technology. 
3. You have at least **5 Project Posts** demonstrating your progress.


See Judging Criteria on [Enter Now Page](https://devart.withgoogle.com/#/enter-now/judging-criteria) for more information on what the judges will be looking for.

##Moderation 

Whenever you update your Project Page with any content, it will be put forward for moderation and will be only visible to you until approved by the DevArt moderators. This process may take some time, so please bear with us. You’ll see a Pending Moderation notification in your Project Page. 

If your post is flagged as including inappropriate content it may be removed. The same applies to any content that is flagged by users as inappropriate content. Entrants who find their Project Page has been removed can delete their forked project in GitHub and re-enter. 

For more information about what is considered as inappropriate content, visit the [Competition Rules](https://devart.withgoogle.com/#/competition-rules).
